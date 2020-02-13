FROM python:3.7.6
RUN mkdir /client
WORKDIR /client

COPY client.py .
COPY requirements.txt . 
RUN pip install -r requirements.txt
ENV HOST "192.168.1.2" 
ENV PORT "8080"
CMD ["python", "client.py"]
