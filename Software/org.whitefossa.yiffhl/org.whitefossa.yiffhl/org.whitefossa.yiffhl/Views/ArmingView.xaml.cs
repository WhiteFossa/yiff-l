using SkiaSharp;
using SkiaSharp.Views.Forms;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace org.whitefossa.yiffhl.Views
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ArmingView : ContentPage
    {
        public ArmingView()
        {
            InitializeComponent();
        }

        private void CanvasView_PaintSurface(object sender, SkiaSharp.Views.Forms.SKPaintSurfaceEventArgs e)
        {
            var info = e.Info;
            var surface = e.Surface;
            var canvas = surface.Canvas;

            canvas.Clear();

            // In this example, we will draw a circle in the middle of the canvas
            var paint = new SKPaint
            {
                Style = SKPaintStyle.Fill,
                Color = Color.Red.ToSKColor(), // Alternatively: SKColors.Red
            };
            canvas.DrawCircle(info.Width / 2, info.Height / 2, 100, paint);
        }
    }
}