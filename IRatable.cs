namespace BookRecommender.Interfaces
{
    /// <summary>
    /// Интерфейс для объектов, которые можно оценивать
    /// </summary>
    public interface IRatable
    {
        double GetAverageRating();
        void AddRating(int score);
    }
}