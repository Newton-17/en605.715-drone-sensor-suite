FROM ubuntu:18.04

RUN apt isntall -y --no-install-recommends gpsd

ENV gpsd_device /dev/ttyAMA0

CMD ["gpsd -N -n -G $gpsd_device"]