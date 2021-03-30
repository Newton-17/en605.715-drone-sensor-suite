FROM ubuntu:18.04 as builder
LABEL maintainer michel.promonet@free.fr

WORKDIR /

ARG ARCH=armv7l
RUN apt-get update \
    && apt install -y --no-install-recommends ca-certificates g++ autoconf automake libtool xz-utils cmake make pkg-config git wget git \
    && git clone https://github.com/mpromonet/v4l2rtspserver.git

WORKDIR /v4l2rtspserver
RUN cmake . \
	&& make install \
	&& apt-get clean && rm -rf /var/lib/apt/lists/


#FROM balenalib/raspberry-pi

#WORKDIR /usr/local/share/v4l2rtspserver
#COPY --from=builder /usr/local/bin/ /usr/local/bin/
#COPY --from=builder /usr/local/share/v4l2rtspserver/ /usr/local/share/v4l2rtspserver/

ENTRYPOINT [ "/usr/local/bin/v4l2rtspserver" ]
CMD [ "-H $frame_height -W $frame_width -I'0.0.0.0' -P $video_port -u 'pi-cam' -F $fps" ]