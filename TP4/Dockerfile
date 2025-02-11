# Utiliser Tomcat comme base
FROM tomcat:latest

# Copier le fichier Jenkins WAR dans le répertoire webapps de Tomcat
COPY jenkins.war /usr/local/tomcat/webapps/

# Exposer le port 8080
EXPOSE 8080

# Lancer Tomcat
CMD ["catalina.sh", "run"]
