# en605.715-drone-sensor-suite
Contains short demos of using a raspberry pi attached to a drone to collect various sensor data

# Default Video Settings:
By Default the Video is accessible at rtsp://<IP>:8554/picam

# Default GPSD Settings:
By Default you can telnet and retrieve GPS by running the following commands:
``` bash
# Connect to GPSD
$ telnet <IP> 2947

# Watch GPSD in JSON -> Paste After the first
$ ?WATCH={"enable":true,"json":true}
```
