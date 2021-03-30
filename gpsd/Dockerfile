FROM ubuntu:18.04

RUN apt update \
    && apt install -y --no-install-recommends gpsd \
    && apt-get clean && rm -rf /var/lib/apt/lists/

ENV gpsd_device /dev/ttyACM0

CMD ["gpsd -N -n -G $gpsd_device"]