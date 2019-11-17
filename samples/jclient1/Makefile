
javac=javac
java=java
jdbc_cp=../mysql-connector-java-5.1.27-bin.jar

jclient1: jclient1.java
	${javac} -cp . jclient1.java

test: jclient1
	${java} -cp ${jdbc_cp}:. jclient1

clean:
	rm -rf jclient1.class


