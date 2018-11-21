pipeline {
    agent any
    stages {
        stage('Pull Beaver') {
            steps {
                dir('beaver') {
                    git([url: 'git://github.com/rainvg/beaver.git', branch: 'master'])
                }
            }
        }
        stage('Launch Beaver') {
            steps {
                dir('beaver') {
                    sh 'npm i'
                    sh 'node ./src/main.js ..'
                }
            }
        }
        stage('Publish Report') {
            steps {
                publishHTML([allowMissing: false, alwaysLinkToLastBuild: false, keepAll: false, reportDir: 'beaver/report', reportFiles: 'index.html', reportName: 'HTML Report', reportTitles: ''])
            }
        }
    }
}
