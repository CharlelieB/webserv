FROM nginx:latest

RUN mkdir /var/www && mkdir /var/www/html && mkdir /var/www/html/website && cd /var/www/html/website/

RUN chmod 755 /var/www/html/website

COPY ./index.html /var/www/html/website/index.html
#COPY ./default.conf /etc/nginx/conf.d/default.conf

CMD ["nginx", "-g", "daemon off;"]

