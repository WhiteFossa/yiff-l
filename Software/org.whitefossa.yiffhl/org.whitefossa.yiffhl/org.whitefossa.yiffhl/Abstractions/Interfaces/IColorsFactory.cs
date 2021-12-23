using SkiaSharp;

namespace org.whitefossa.yiffhl.Abstractions.Interfaces
{
    /// <summary>
    /// Factory to generate various colours depending on app theme
    /// </summary>
    public interface IColorsFactory
    {
        /// <summary>
        /// Color for main lines and texts
        /// </summary>
        SKColor GetMainColor();

        /// <summary>
        /// Color for secondary (pale) lines
        /// </summary>
        SKColor GetSecondaryColor();

        /// <summary>
        /// Color for points on antenna matching graph
        /// </summary>
        SKColor GetMatchingPointsColor();

        /// <summary>
        /// Color for lines between points on antenna matching graph
        /// </summary>
        SKColor GetMatchingPointsLinesColor();

        /// <summary>
        /// Color for lines, intersecting at best matching point
        /// </summary>
        SKColor GetBestMatchingPointLinesColor();
    }
}
