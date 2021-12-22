using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Events;
using org.whitefossa.yiffhl.Models;
using org.whitefossa.yiffhl.ViewModels;
using SkiaSharp;
using SkiaSharp.Views.Forms;
using System.Threading.Tasks;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace org.whitefossa.yiffhl.Views
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ArmingView : ContentPage
    {
        private ArmingViewModel ViewModel
        {
            get => BindingContext as ArmingViewModel;
            set => BindingContext = value;
        }

        public ArmingView()
        {
            InitializeComponent();

            // Attaching to events
            var packetsProcessor = App.Container.Resolve<IPacketsProcessor>();

            ViewModel.MainModel.OnFoxDisarmed += async (e) => await OnFoxDisarmedAsync(e);
            packetsProcessor.RegisterOnFoxDisarmedEventsHandler(ViewModel.MainModel.OnFoxDisarmed);
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

        protected override bool OnBackButtonPressed()
        {
            if (ViewModel.MainModel.ArmingModel.Status != ArmingStatus.Completed)
            {
                return true; // We can't interrupt arming process
            }

            Navigation.PopModalAsync();
            return true;
        }

        private async Task OnFoxDisarmedAsync(IFoxDisarmedEvent foxDisarmedEvent)
        {
            await Navigation.PopModalAsync();
        }
    }
}