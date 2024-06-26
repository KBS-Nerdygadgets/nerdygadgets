package TSP;

import java.util.ArrayList;
import java.util.List;

import Database.Database;

public class Main {
    //Het nulpunt van de robot, dit moet het punt waar de robot begint worden
    Point nul = new Point(1, 5);

    //Collectie van functies voor het TSP proces
    public static ArrayList<ArrayList<Integer>> TSPfuncties(ArrayList<ArrayList<Integer>> BPParray, int OrderID){
        ArrayList<ArrayList<Integer>> locaties = Database.fetchLocations(BPParray, OrderID);
        System.out.println(locaties);
        ArrayList<ArrayList<Point>> points2DArray = locatieOmzettenNaarPoints(locaties);
        ArrayList<ArrayList<Point>> sorteerPoints2DArray = sorteren(points2DArray);
        return pointsOmzettenNaarLocatie(sorteerPoints2DArray);
    }

    //De locatie in het 2D array worden omgezet naar punten
    public static ArrayList<ArrayList<Point>> locatieOmzettenNaarPoints(ArrayList<ArrayList<Integer>> locaties){
        ArrayList<ArrayList<Point>> locatiesNaarPoints = new ArrayList<>();

        int[][] coordinates = {
            {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5},
            {1, 4}, {2, 4}, {3, 4}, {4, 4}, {5, 4},
            {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3},
            {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2},
            {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}
        };

        for(int rit = 0; rit < locaties.size(); rit++){
            locatiesNaarPoints.add(new ArrayList<>());
            for(int locatie = 0; locatie < locaties.get(rit).size(); locatie++){
                int locatieTemp = locaties.get(rit).get(locatie);
                Point tempPoint = new Point(coordinates[locatieTemp][0], coordinates[locatieTemp][1]);
                locatiesNaarPoints.get(rit).add(tempPoint);
            }
        }
        System.out.println("TSP: Locaties omgezet naar Points");
        return locatiesNaarPoints;
    }
    
    //De punten worden terug omgezet naar locaties
    public static ArrayList<ArrayList<Integer>> pointsOmzettenNaarLocatie(ArrayList<ArrayList<Point>> points){
        ArrayList<ArrayList<Integer>> locaties = new ArrayList<>();

        int[][] locatiesArray= {
            {20, 15, 10, 5, 0},
            {21, 16, 11, 6, 1},
            {22, 17, 12, 7, 2},
            {23, 18, 13, 8, 3},
            {24, 19, 14, 9, 4}
        };

        for(int rit = 0; rit < points.size(); rit++){
            locaties.add(new ArrayList<>());
            for(int point = 0; point < points.get(rit).size(); point++){
                int x = (points.get(rit).get(point).getX()) - 1;
                int y = (points.get(rit).get(point).getY()) - 1;
                locaties.get(rit).add(locatiesArray[x][y]);
            }
        }
        System.out.println("TSP: Points terug naar Locaties");
        return locaties;
    }

    //De points worden gesorteerd door het algoritme
    public static ArrayList<ArrayList<Point>> sorteren(ArrayList<ArrayList<Point>> points2DArray){
        //Elke rit moet gesorteerd worden
        for(int rit = 0; rit < points2DArray.size(); rit++){
            ArrayList<Point> pointsArray = points2DArray.get(rit);
            // Hier wordt de nieuwe array 'points' aangemaakt.
            // Aan de array 'points' worden de punten van 'pointsArray' toegevoegd
            List<Point> points = new ArrayList<>();
            for (Point p : pointsArray){
                points.add(p);
                }

            // Het startpunt van de robot
            Point start = points.remove(0);

            // Hier wordt het Nearest Neighbor-algoritme toegepast
            List<Point> path = NearestNeighbor.nearestNeighborAlgorithm(start, points);

            for(int i = 0; i < path.size(); i++){
                path.get(i);
                points2DArray.get(rit).set(i, path.get(i));
            }
        }
        System.out.println("TSP: Points gesorteerd");
        return points2DArray;
    }
}
