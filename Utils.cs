using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace BookRecommender.Services
{
    /// <summary>
    /// Вспомогательные утилиты
    /// </summary>
    public static class Utils
    {
        /// <summary>
        /// Простое хеширование паролей (для учебных целей)
        /// </summary>
        public static string SimpleHash(string input)
        {
            if (string.IsNullOrEmpty(input))
                return string.Empty;

            ulong hash = 5381;
            foreach (char c in input)
            {
                hash = ((hash << 5) + hash) + (ulong)c;
            }
            return hash.ToString();
        }

        /// <summary>
        /// Создание директории для данных
        /// </summary>
        public static bool EnsureDataDir()
        {
            try
            {
                if (!Directory.Exists("data"))
                {
                    Directory.CreateDirectory("data");
                }
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// Разбиение строки по разделителю
        /// </summary>
        public static List<string> Split(string str, char delimiter)
        {
            if (string.IsNullOrEmpty(str))
                return new List<string>();
            return str.Split(delimiter).ToList();
        }

        /// <summary>
        /// Сохранение списка в файл
        /// </summary>
        public static void SaveToFile<T>(string filename, List<T> items)
        {
            try
            {
                EnsureDataDir();
                using (var writer = new StreamWriter($"data/{filename}"))
                {
                    writer.WriteLine(items.Count);
                    foreach (var item in items)
                    {
                        writer.WriteLine(item?.ToString() ?? "null");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error saving to {filename}: {ex.Message}");
            }
        }

        /// <summary>
        /// Загрузка списка из файла
        /// </summary>
        public static List<string> LoadFromFile(string filename)
        {
            var result = new List<string>();
            try
            {
                if (File.Exists($"data/{filename}"))
                {
                    using (var reader = new StreamReader($"data/{filename}"))
                    {
                        var line = reader.ReadLine();
                        if (int.TryParse(line, out int count))
                        {
                            for (int i = 0; i < count; i++)
                            {
                                result.Add(reader.ReadLine() ?? string.Empty);
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error loading from {filename}: {ex.Message}");
            }
            return result;
        }

        /// <summary>
        /// Очистка консольного ввода
        /// </summary>
        public static string ReadLineSafe()
        {
            var input = Console.ReadLine();
            return input ?? string.Empty;
        }

        /// <summary>
        /// Чтение числа с проверкой
        /// </summary>
        public static bool TryReadInt(out int result)
        {
            return int.TryParse(Console.ReadLine(), out result);
        }

        /// <summary>
        /// Чтение числа с проверкой и сообщением об ошибке
        /// </summary>
        public static int ReadInt(string prompt, int? min = null, int? max = null)
        {
            while (true)
            {
                Console.Write(prompt);
                if (int.TryParse(Console.ReadLine(), out int result))
                {
                    if (min.HasValue && result < min.Value)
                    {
                        Console.WriteLine($"Значение должно быть не меньше {min.Value}");
                        continue;
                    }
                    if (max.HasValue && result > max.Value)
                    {
                        Console.WriteLine($"Значение должно быть не больше {max.Value}");
                        continue;
                    }
                    return result;
                }
                Console.WriteLine("Неверный формат числа. Попробуйте снова.");
            }
        }

        /// <summary>
        /// Чтение числа с плавающей точкой
        /// </summary>
        public static double ReadDouble(string prompt, double? min = null, double? max = null)
        {
            while (true)
            {
                Console.Write(prompt);
                if (double.TryParse(Console.ReadLine(), out double result))
                {
                    if (min.HasValue && result < min.Value)
                    {
                        Console.WriteLine($"Значение должно быть не меньше {min.Value}");
                        continue;
                    }
                    if (max.HasValue && result > max.Value)
                    {
                        Console.WriteLine($"Значение должно быть не больше {max.Value}");
                        continue;
                    }
                    return result;
                }
                Console.WriteLine("Неверный формат числа. Попробуйте снова.");
            }
        }
    }
}