FROM ubuntu:18.04

COPY ./start.sh /start.sh

RUN apt update \
    && apt install -y --no-install-recommends gpsd \
    && apt-get clean && rm -rf /var/lib/apt/lists/ \
    && chmod 777 /src/start.sh


ENV gpsd_device /dev/ttyACM0

CMD ["bash", "./start.sh"]