package TSP;

import java.util.ArrayList;
import java.util.List;
import Database.Database;

public class Main {
    public static List<Point> fetchPoints() {
        // Fetch points from the database
        List<Point> points = Database.fetchPointsFromDatabase();
        return points;
    }
    public static void main(String[] args) {
        // Points ophalen van de database
        List<Point> points = Database.fetchPointsFromDatabase();

        // Als er geen points zijn, stoppen
        if (points.isEmpty()) {
            System.out.println("No points found in the database.");
            return;
        }

        // Het start point van de robot
        Point start = points.remove(0);

        // Het Nearest Neighbour Algorithm toepassen
        List<Point> path = NearestNeighbor.nearestNeighborAlgorithm(start, points);

        // Het gevonden pad printen
        System.out.println("Gevonden pad:");
        for (Point p : path) {
            System.out.println("Punt: (" + p.x + ", " + p.y + ")");
        }
    }
}