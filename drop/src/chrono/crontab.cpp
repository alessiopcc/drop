// Includes

#include "crontab.h"

namespace drop
{
    // crontab

    // Constructors

    crontab :: crontab() : _waketime(0), _alive(true), _thread(&crontab :: run, this)
    {
    }

    // Destructors

    crontab :: ~crontab()
    {
        this->_alive = false;
        this->_semaphore.post();
        this->_thread.join();
    }

    // Methods

    promise <void> crontab :: wait(const timestamp & target)
    {
        event event(target);

        this->_guard([&]()
        {
            this->_pending.push_back(event);
            if((this->_waketime == 0) || (target + settings :: tick < this->_waketime))
            {
                this->_waketime = 1;
                this->_semaphore.post();
            }
        });

        return event;
    }

    // Private methods

    void crontab :: update(const timestamp & target)
    {
        static thread_local std :: vector <event> expired;

        this->_guard([&]()
        {
            for(const auto & event : this->_pending)
            {
                if(event < target)
                    expired.push_back(event);
                else
                    this->_events.push(event);
            }

            this->_pending.clear();
        });

        for(const auto & event : expired)
            event.resolve();

        expired.clear();
    }

    void crontab :: flush(const timestamp & target)
    {
        while(!this->_events.empty() && this->_events.top() < target)
        {
            this->_events.top().resolve();
            this->_events.pop();
        }
    }

    void crontab :: sleep(const timestamp & target)
    {
        timestamp waketime;

        this->_guard([&]()
        {
            if(this->_events.empty())
                this->_waketime = 0;
            else
                this->_waketime = std :: max((const timestamp &) (this->_events.top()), target + settings :: tick);

            waketime = this->_waketime;
        });

        if(waketime > 0)
            this->_semaphore.wait(waketime);
        else
            this->_semaphore.wait();
    }

    void crontab :: run()
    {
        while(this->_alive)
        {
            timestamp target = now();

            this->_guard([&]()
            {
                this->_waketime = 0;
            });

            this->update(target);
            this->flush(target);
            this->sleep(target);
        }
    }

    // event

    // Constructors

    crontab :: event :: event(const timestamp & time) : timestamp(time)
    {
    }
};
