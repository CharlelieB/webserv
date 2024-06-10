FROM nginx:latest

RUN mkdir /var/www && mkdir /var/www/html && mkdir /var/www/html/website && cd /var/www/html/website/ && mkdir /var/www/html/upload
RUN chmod 755 /var/www/html/website && chown -R www-data:www-data /var/www/html/upload && chmod -R 755 /var/www/html/upload

COPY ./index.html /var/www/html/website/index.html
COPY ./default.conf /etc/nginx/conf.d/default.conf

EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]