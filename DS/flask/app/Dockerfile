FROM python:3.7.6
RUN mkdir /app
WORKDIR /app
COPY app.py /app
COPY requirements.txt . 
RUN pip install -r requirements.txt
EXPOSE 8080
ENV ID "2"
CMD ["python", "app.py"]
