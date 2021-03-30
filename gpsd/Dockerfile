FROM linarotechnologies/alpine:edge

RUN apk add --no-cache gpsd

ENV gpsd_device /dev/ttyAMA0

CMD ["gpsd -N -n -G $gps_interface_path"]