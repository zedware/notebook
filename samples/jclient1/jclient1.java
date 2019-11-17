import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class jclient1 {
  public static void main(String[] args) {
  Connection conn = null;
  Statement stmt = null;
  ResultSet rs = null;
  String url = null;
  String user = null;
  String password = null;
  String sql = null;
  try {
   Class.forName("com.mysql.jdbc.Driver");
  } catch (ClassNotFoundException e) {
   System.out.println("cannot load driver");
   e.printStackTrace();
  }
  try {
   url = 
    "jdbc:mysql://127.0.0.1:4006/test?user=root&password=&useUnicode=true&&characterEncoding=gbk&autoReconnect=true";
   user = "root";
   password = "";
   conn = DriverManager.getConnection(url,user,password);
  } catch (SQLException e) {
   System.out.println("cannot connect to database");
   e.printStackTrace();
  }

  try {
//   conn.setAutoCommit(false);

   stmt = conn.createStatement();
   sql = "begin";
   rs = stmt.executeQuery(sql);

   stmt = conn.createStatement();
   sql = "update COMMIT_ON_SUCCESS x set xname='x' where xno=3;";
   stmt.execute(sql);
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
   System.out.println("select error");
   e.printStackTrace();
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
