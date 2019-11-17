import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class jclient2 {
  public static void main(String[] args) {
    Connection conn = null;
    Statement stmt = null;
    ResultSet rs = null;
    String url = null;
    String user = null;
    String password = null;
    String sql = null;
    int rc = 0;

    try {
      Class.forName("com.mysql.jdbc.Driver");
    } catch (ClassNotFoundException e) {
      System.out.println("cannot load driver");
      e.printStackTrace();
    }

    try {
      url = "jdbc:mysql://127.0.0.1:4006/test?user=root&password=&useUnicode=true&&allowMultiQueries=true&autoReconnect=true";
      user = "root";
      password = "";
      conn = DriverManager.getConnection(url,user,password);
    } catch (SQLException e) {
      System.out.println("cannot connect to database");
      e.printStackTrace();
    }

    /*
Create Table: CREATE TABLE `foo` (
  `fno` int(11) NOT NULL DEFAULT '0',
  `fname` char(8) DEFAULT NULL,
  PRIMARY KEY (`fno`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk

INSERT INTO foo VALUES(1, 'James');
INSERT INTO foo VALUES(2, 'Bill');
INSERT INTO foo VALUES(3, 'Tom');

    */
    try {
      conn.setAutoCommit(false);

      stmt = conn.createStatement(java.sql.ResultSet.TYPE_FORWARD_ONLY, java.sql.ResultSet.CONCUR_UPDATABLE);
      sql = "UPDATE foo SET fname='JamesP' WHERE fno=1;UPDATE foo SET fno=1 WHERE fno=2;UPDATE SET fname='TomZ' WHERE fno=3";
      rc  = stmt.executeUpdate(sql);

  /*
     stmt = conn.createStatement();
     sql = "select @@port";
     rs = stmt.executeQuery(sql);
     while(rs.next()) {
      System.out.print(rs.getString(1) + "   ");
      // System.out.print(rs.getString(2) + "   ");
     }
  */
      conn.commit();
    } catch (SQLException e) {
      System.out.println("SQL error");
      e.printStackTrace();
      System.out.println("SQLState: " + e.getSQLState());
      System.out.println("SQLCode: " + e.getErrorCode());
      System.out.println("Message: " + e.getMessage());
    }

    System.out.println();

    try {
      if(rs != null) {
        rs.close();
        rs = null;
      }

      if(stmt != null) {
        stmt.close();
        stmt = null;
      }

      if(conn != null) {
        conn.close();
        conn = null;
      }
    } catch(Exception e) {
      System.out.println("cannot close database");
      e.printStackTrace();
    }
  }
}
