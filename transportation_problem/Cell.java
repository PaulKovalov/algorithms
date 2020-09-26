
/**
 *
 * @author paul
 */
public class Cell implements Comparable<Cell> {

    public double tariff, usedAmount = 0;
    public int producer, consumer, flag;
    public boolean used = false;

    public Cell(int producer, int consumer, double tariff) {
        this.producer = producer;
        this.consumer = consumer;
        this.tariff = tariff;
    }

    public Cell(Cell other) {
        this(other.producer, other.consumer, other.tariff);
        this.flag = other.flag;
        this.used = other.used;
        this.usedAmount = other.usedAmount;
    }

    @Override
    public int compareTo(Cell otherCell) {
        if (this.tariff < otherCell.tariff) {
            return -1;
        } else if (this.tariff == otherCell.tariff) {
            return 0;
        }
        return 1;
    }

    public boolean canBeFlagged(Cell other) {
        if (this.equals(other)) {
            return false;
        }
        if (this.usedAmount == 0) {
            return false;
        }
        return !this.used;
    }

    @Override
    public int hashCode() {
        return super.hashCode();
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Cell other = (Cell) obj;
        if (this.producer != other.producer) {
            return false;
        }
        return this.consumer == other.consumer;
    }
}
