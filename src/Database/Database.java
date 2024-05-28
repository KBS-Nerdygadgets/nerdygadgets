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

    public static ArrayList<ArrayList<Integer>> fetchLocations(ArrayList<ArrayList<Integer>> BPPLijst, int OrderID) {
        ArrayList<ArrayList<Integer>> locations = new ArrayList<>();

        // JDBC Connection Parameters
        String url = "jdbc:mysql://localhost:3306/nerdygadgets";
        String username = "root";
        String password = "";

        for(int rit = 0; rit < BPPLijst.size(); rit++){
            locations.add(new ArrayList<>());
            for(int itemIndex = 0; itemIndex < BPPLijst.get(rit).size(); itemIndex++){
                // SQL query
                String query = "SELECT Location from orderlines WHERE StockItemID = "+ BPPLijst.get(rit).get(itemIndex) + " AND OrderID = " + OrderID;

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
                        int locatie = resultSet.getInt("Location");
                        System.out.println(locatie);

                        // Nieuw Point aanmaken en toevoegen aan de lijst
                        locations.get(rit).add(locatie);
                    }

                    // Exit resources
                    resultSet.close();
                    statement.close();
                    connection.close();
                }
                catch (SQLException e) {
                    e.printStackTrace();
                }
            }
        }
        return locations;
    }
}
