FROM node

WORKDIR /server
COPY package.json /server/
RUN npm install
COPY . /server

RUN set -xe \
    && mkdir -vp /server/db/

CMD []
ENTRYPOINT ["nodejs", "main"]

EXPOSE 8000
