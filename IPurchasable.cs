namespace BookRecommender.Interfaces
{
    /// <summary>
    /// Интерфейс покупаемого товара
    /// </summary>
    public interface IPurchasable
    {
        double GetPrice();
        string ToDisplayString();
        double ApplyDiscount(double discountPercent);
    }
}