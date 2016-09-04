#!/bin/bash

function opengrok-reindex () {
    # wait for tomcat to be ready.
    while ! (ss -tln '( sport = :2424 )'|grep ^LISTEN); do
        sleep 1
    done
    echo "*** Updating index"
    OpenGrok index

#    while ! [ -z $OPENGROK_REINDEX ] ; do
#        inotifywait -mr -e CLOSE_WRITE /src | while read dir mode file; do
#            echo "$dir/$file upadted"
#            echo "*** Updating index"
#            OpenGrok index
#        done
#    done
}
ln -fs /src /var/opengrok/src
opengrok-reindex &
JAVA_OPTS="-Djava.awt.headless=true -Xmx128M -Djava.net.preferIPv4Stack=true -Djava.net.preferIPv4Addresses=true" /usr/share/tomcat8/bin/catalina.sh run &
sleep 5
/root/OpenGrok/OpenGrok deploy
/bin/bash
