using org.whitefossa.yiffhl.Abstractions.Interfaces;
using SkiaSharp;
using Xamarin.Essentials;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class ColorsFactory : IColorsFactory
    {
        private bool _isDark;

        public ColorsFactory()
        {
            _isDark = AppInfo.RequestedTheme == AppTheme.Dark;

        }

        public SKColor GetMainColor()
        {
            return _isDark ? SKColors.White : SKColors.Black;
        }

        public SKColor GetSecondaryColor()
        {
            return _isDark ? SKColors.Gray : SKColors.Gray;
        }

        public SKColor GetMatchingPointsColor()
        {
            return _isDark ? SKColors.LightGreen : SKColors.Red;
        }

        public SKColor GetMatchingPointsLinesColor()
        {
            return _isDark ? SKColors.LightGreen : SKColors.Red;
        }

        public SKColor GetBestMatchingPointLinesColor()
        {
            return _isDark ? SKColors.LightBlue : SKColors.Blue;
        }
    }
}
