package Bin_Packing;

import Database.Database;

public class Product {
    int stockItemID;
    int gewicht;


    public Product(int stockItemID) {
        this.stockItemID = stockItemID;
        this.gewicht = Database.fetchGewichtFromDatabase(stockItemID);
    }

    public String toString() {
        return "StockItemID " + stockItemID + ".";
    }
}
