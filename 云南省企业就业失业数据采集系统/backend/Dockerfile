FROM openjdk:21

WORKDIR /app
COPY . /app

RUN chmod +x mvnw
RUN ./mvnw clean package
RUN cp target/*.jar app.jar

ENTRYPOINT ["java", "-jar", "app.jar"]