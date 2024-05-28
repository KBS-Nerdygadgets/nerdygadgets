package Bin_Packing;
import java.util.ArrayList;

public class Bin {
    int capaciteit;
    int ruimte;
    ArrayList<Product> producten;

    public Bin(int capaciteit) {
        this.capaciteit = capaciteit;
        ruimte = capaciteit;
        producten = new ArrayList<Product>();
    }

    public boolean voegItemToe(Product product){
        if(ruimte >= product.gewicht && producten.size() < 3){
            ruimte -= product.gewicht;
            this.producten.add(product);
            return true;
        }
        return false;
    }
}
