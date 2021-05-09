FROM ubuntu:18.04 as builder
LABEL maintainer michel.promonet@free.fr

WORKDIR /

RUN apt-get update \
    && apt install -y --no-install-recommends ca-certificates g++ autoconf automake libtool xz-utils cmake make pkg-config git wget git \
    && git clone https://github.com/mpromonet/v4l2rtspserver.git \
    && cd v4l2rtspserver \
    && cmake . \
	&& make install \
	&& apt-get clean && rm -rf /var/lib/apt/lists/

ENV video_port 8554
ENV frame_width 640	
ENV frame_height 480
ENV fps 30

CMD [ "sh", "-c", "/usr/local/bin/v4l2rtspserver -H $frame_height -W $frame_width -I'0.0.0.0' -P $video_port -u'picam' -F $fps" ]
