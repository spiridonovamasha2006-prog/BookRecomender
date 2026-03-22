namespace BookRecommender.Interfaces
{
    /// <summary>
    /// Интерфейс для объектов, которые можно искать
    /// </summary>
    public interface ISearchable
    {
        bool Matches(string query);
    }
}