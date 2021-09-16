
pipeline {

    agent {

        dockerfile {
            filename 'Dockerfile'
        }

    }

    stages {

        stage('Build') {

            parallel {

                stage('Build Debug') {

                    steps {

                        sh '''
                            CC=clang CXX=clang++ cmake \\
                                -DCMAKE_BUILD_TYPE=Debug \\
                                -B cmake_build_Debug -S . \\
                                -G 'Unix Makefiles'
                        '''
                        sh 'make -C cmake_build_Debug'

                    }

                }

                stage('Build Release') {

                    steps {

                        sh '''
                            CC=clang CXX=clang++ cmake \\
                                -DCMAKE_BUILD_TYPE=Release \\
                                -B cmake_build_Release -S . \\
                                -G 'Unix Makefiles'
                        '''
                        sh 'make -C cmake_build_Release'

                    }

                }

                stage('Build MinSizeRel') {

                    steps {

                        sh '''
                            CC=clang CXX=clang++ cmake \\
                                -DCMAKE_BUILD_TYPE=MinSizeRel \\
                                -B cmake_build_MinSizeRel -S . \\
                                -G 'Unix Makefiles'
                        '''
                        sh 'make -C cmake_build_MinSizeRel'

                    }

                }

                stage('Build RelWithDebInfo') {

                    steps {

                        sh '''
                            CC=clang CXX=clang++ cmake \\
                                -DCMAKE_BUILD_TYPE=RelWithDebInfo \\
                                -B cmake_build_RelWithDebInfo -S . \\
                                -G 'Unix Makefiles'
                        '''
                        sh 'make -C cmake_build_RelWithDebInfo'

                    }

                }

            }

        }

        // Test here when supported

        stage('Generate Docs') {

            steps {

                sh 'doxygen Doxyfile'

            }

        }

    }

}