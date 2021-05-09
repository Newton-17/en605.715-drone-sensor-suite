FROM python:3.7.10-slim

WORKDIR /src
COPY ./* /src/

RUN pip install -r /src/requirements.txt

CMD ["python", "/src/main.py"]