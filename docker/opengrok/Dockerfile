FROM docker.io/ubuntu
RUN apt-get -y update
RUN apt-get -y install tomcat8 tomcat8-admin tomcat8-common tomcat8-user git net-tools openjdk-8-jre openjdk-8-jdk ant ctags

RUN mkdir /usr/share/tomcat8/logs
RUN mkdir /usr/share/tomcat8/conf
RUN cp /etc/tomcat8/*.xml /usr/share/tomcat8/conf/.

RUN cd ~ && git clone https://github.com/OpenGrok/OpenGrok.git
RUN cd ~/OpenGrok && ant

RUN mkdir -p /var/opengrok

ENV PATH=/root/OpenGrok:$PATH \
    OPENGROK_TOMCAT_BASE=/usr/share/tomcat8 \
    OPENGROK_VERBOSE=true

RUN mkdir /usr/share/tomcat8/temp
WORKDIR $OPENGROK_TOMCAT_BASE
RUN /root/OpenGrok/OpenGrok deploy

RUN mkdir /src \
 && mkdir /data

EXPOSE 8080
ADD entry.sh /entry.sh
CMD ["/entry.sh"]
