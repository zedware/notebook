
javac=javac
java=java
jdbc_cp=../mysql-connector-java-5.1.27-bin.jar

jclient2: jclient2.java
	${javac} -cp . jclient2.java

test: jclient2
	${java} -cp ${jdbc_cp}:. jclient2

clean:
	rm -rf jclient2.class


