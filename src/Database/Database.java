package Database;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;
import TSP.Point;

public class Database {
    public static List<Point> fetchPointsFromDatabase() {
        List<Point> points = new ArrayList<>();

        // JDBC Connection Parameters
        String url = "jdbc:mysql://localhost:3306/nerdygadgets";
        String username = "root";
        String password = "";

        // SQL query
        String query = "SELECT * FROM warehouse";

        try {
            // Connectie maken
            Connection connection = DriverManager.getConnection(url, username, password);

            // Create statement
            Statement statement = connection.createStatement();

            // Execute query
            ResultSet resultSet = statement.executeQuery(query);

            // Resultaten processen
            while (resultSet.next()) {
                // Data ophalen van de set
                int x = resultSet.getInt("X");
                int y = resultSet.getInt("Y");

                // Nieuw Point aanmaken en toevoegen aan de lijst
                Point point = new Point(x, y);
                points.add(point);
            }

            // Exit resources
            resultSet.close();
            statement.close();
            connection.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return points;
    }

    public static int fetchGewichtFromDatabase(int stockItemID) {
        int gewicht = 0;

        // JDBC Connection Parameters
        String url = "jdbc:mysql://localhost:3306/nerdygadgets";
        String username = "root";
        String password = "";

        // SQL query
        String query = "SELECT * FROM orderlines WHERE StockItemID = " + stockItemID + " && OrderID < 9";

        try {
            // Establish connection
            Connection connection = DriverManager.getConnection(url, username, password);

            // Create statement
            Statement statement = connection.createStatement();

            // Execute query
            ResultSet resultSet = statement.executeQuery(query);

            // Process results
            while (resultSet.next()) {
                // Retrieve data from the result set
                gewicht = resultSet.getInt("Weight");
            }

            // Exit resources
            resultSet.close();
            statement.close();
            connection.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return gewicht;
    }

    public static List<Integer> fetchStockItemIDFromDatabase(int chosenOrderID) {
        List<Integer> productIDs  = new ArrayList<>();

        // JDBC Connection Parameters
        String url = "jdbc:mysql://localhost:3306/nerdygadgets";
        String username = "root";
        String password = "";

        // SQL query
        String query = "SELECT StockItemID FROM orderlines WHERE OrderID = " + chosenOrderID + " && OrderID < 9";

        try {
            // Establish connection
            Connection connection = DriverManager.getConnection(url, username, password);

            // Create statement
            Statement statement = connection.createStatement();

            // Execute query
            ResultSet resultSet = statement.executeQuery(query);

            // Process results
            while (resultSet.next()) {
                // Retrieve data from the result set
                int productID = resultSet.getInt("StockItemID");
                productIDs.add(productID);
            }

            // Exit resources
            resultSet.close();
            statement.close();
            connection.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return productIDs;
    }
}
