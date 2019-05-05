#!/bin/bash
if [[ $TRAVIS_COMMIT_MESSAGE == *"[skip deploy]"* ]]
then
	echo "Skipping deploy stage"
else	
	mkdir -p ~/.ssh/
	touch ~/.ssh/config
	echo -e "Host *\n\tStrictHostKeyChecking no\n" >> ~/.ssh/config
	openssl aes-256-cbc -K $encrypted_ce33e766463c_key -iv $encrypted_ce33e766463c_iv -in deploy_key.enc -out deploy_key -d
	eval "$(ssh-agent -s)"
	chmod 600 deploy_key
	ssh-add deploy_key
	ssh -i deploy_key $FEENK_CLOUD pwd

	cd build

	if [[ $TRAVIS_OS_NAME == "osx" ]]; then
		scp libMoz2D.dylib $FEENK_CLOUD:/var/www/html/Moz2D/osx/development/x86_64
	fi
	if [[ $TRAVIS_OS_NAME == "linux" ]]; then
		scp libMoz2D.so $FEENK_CLOUD:/var/www/html/Moz2D/linux/development/x86_64
	fi
	
	if [[ $TRAVIS_OS_NAME == "windows" ]]; then
		scp Release/Moz2D.dll $FEENK_CLOUD:/var/www/html/Moz2D/windows/development/x86_64/libMoz2D.dll
	fi
	
	ssh-agent -k
fi
