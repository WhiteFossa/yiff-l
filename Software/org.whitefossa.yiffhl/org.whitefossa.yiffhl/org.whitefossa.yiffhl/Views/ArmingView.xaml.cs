using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Events;
using org.whitefossa.yiffhl.Models;
using org.whitefossa.yiffhl.ViewModels;
using SkiaSharp;
using SkiaSharp.Views.Forms;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace org.whitefossa.yiffhl.Views
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class ArmingView : ContentPage
    {
        /// <summary>
        /// How many matching positions do we have
        /// </summary>
        private const int NumberOfMatchingPositions = 64;

        private const int BordersSpan = 10;

        private const int MatchingPointRadius = 5;

        private const float AutoScaleMaxVoltageGraphHeight = 0.9f;

        private const int TextPadding = 10;

        private const int TextSize = 40;

        private const int StrokeThin = 1;

        private const int StrokeThick = 3;

        private IColorsFactory _colorsFactory;

        private ArmingViewModel ViewModel
        {
            get => BindingContext as ArmingViewModel;
            set => BindingContext = value;
        }

        public ArmingView()
        {
            _colorsFactory = App.Container.Resolve<IColorsFactory>();

            InitializeComponent();

            // Attaching to events
            var packetsProcessor = App.Container.Resolve<IPacketsProcessor>();

            ViewModel.MainModel.OnFoxDisarmed += async (e) => await OnFoxDisarmedAsync(e);
            packetsProcessor.RegisterOnFoxDisarmedEventHandler(ViewModel.MainModel.OnFoxDisarmed);

            ViewModel.MainModel.OnAntennaMatchingMeasurement += async (e) => await OnAntennaMatchingMeasurementAsync(e);
            packetsProcessor.RegisterOnAntennaMatchingMeasurementEventHandler(ViewModel.MainModel.OnAntennaMatchingMeasurement);
        }

        private void MatchingGraphView_PaintSurface(object sender, SkiaSharp.Views.Forms.SKPaintSurfaceEventArgs e)
        {
            var info = e.Info;
            var canvas = e.Surface.Canvas;

            canvas.Clear();

            // Main (dark assuming white theme) paint
            var mainPaint = new SKPaint
            {
                Style = SKPaintStyle.Stroke,
                Color = _colorsFactory.GetMainColor(),
                StrokeWidth = StrokeThick
            };

            // Secondary (pale lines) paint
            var secondaryPaint = new SKPaint
            {
                Style = SKPaintStyle.Stroke,
                Color = _colorsFactory.GetSecondaryColor(),
                StrokeWidth = StrokeThin
            };

            // Matching points
            var matchingPointsPaint = new SKPaint
            {
                Style = SKPaintStyle.StrokeAndFill,
                Color = _colorsFactory.GetMatchingPointsColor(),
                StrokeWidth = StrokeThin
            };

            // Lines between matching points
            var matchingPointsLinesPaint = new SKPaint
            {
                Style = SKPaintStyle.StrokeAndFill,
                Color = _colorsFactory.GetMatchingPointsLinesColor(),
                StrokeWidth = StrokeThick
            };

            // Best matching lines
            var bestMatchingLinesPaint = new SKPaint
            {
                Style = SKPaintStyle.StrokeAndFill,
                Color = _colorsFactory.GetBestMatchingPointLinesColor(),
                StrokeWidth = StrokeThick
            };

            // Text paint
            var textPaint = new SKPaint
            {
                Style = SKPaintStyle.StrokeAndFill,
                Color = _colorsFactory.GetMainColor(),
                StrokeWidth = StrokeThin,
                TextSize = TextSize
            };

            // Borders
            var bordersRect = new SKRect(BordersSpan, BordersSpan, info.Width - BordersSpan, info.Height - BordersSpan);
            canvas.DrawRect(bordersRect, mainPaint);

            // Vertical (matching) lines
            for (var matchingPosition = 0; matchingPosition < NumberOfMatchingPositions; matchingPosition ++)
            {
                var x = MatcherPositionToX(bordersRect, matchingPosition);
                canvas.DrawLine(x, bordersRect.Bottom, x, bordersRect.Top, secondaryPaint);
            }

            // Matching points
            if (ViewModel.MainModel.ArmingModel.OrderdMatchingData != null)
            {
                var maxValue = ViewModel.MainModel.ArmingModel.OrderdMatchingData
                    .Max(kv => kv.Value);

                if (maxValue == 0)
                {
                    maxValue = 0.0000001f;
                }

                var yScale = bordersRect.Height * AutoScaleMaxVoltageGraphHeight / maxValue;

                KeyValuePair<int, float> previousPoint;
                foreach (var currentPoint in ViewModel.MainModel.ArmingModel.OrderdMatchingData)
                {
                    // Point itself
                    var x = MatcherPositionToX(bordersRect, currentPoint.Key);
                    var y = AntennaVoltageToY(bordersRect, yScale, currentPoint.Value);

                    canvas.DrawCircle(x, y, MatchingPointRadius, matchingPointsPaint);

                    // Connecting line
                    if (currentPoint.Key != ViewModel.MainModel.ArmingModel.OrderdMatchingData.First().Key)
                    {
                        var previousX = MatcherPositionToX(bordersRect, previousPoint.Key);
                        var previousY = AntennaVoltageToY(bordersRect, yScale, previousPoint.Value);

                        canvas.DrawLine(x, y, previousX, previousY, matchingPointsLinesPaint);
                    }

                    previousPoint = currentPoint;
                }

                // Best matching
                var bestMatchingX = MatcherPositionToX(bordersRect, ViewModel.MainModel.ArmingModel.BestMatchingPosition);
                var bestMatchingY = AntennaVoltageToY(bordersRect, yScale, ViewModel.MainModel.ArmingModel.BestMatchingPositionVoltage);

                canvas.DrawLine(bordersRect.Left, bestMatchingY, bordersRect.Right, bestMatchingY, bestMatchingLinesPaint);
                canvas.DrawLine(bestMatchingX, bordersRect.Top, bestMatchingX, bordersRect.Bottom, bestMatchingLinesPaint);

                // Voltage (at horizontal line)
                var voltageString = $"{ ViewModel.MainModel.ArmingModel.BestMatchingPositionVoltage }V";

                var voltageBounds = new SKRect();
                textPaint.MeasureText(voltageString, ref voltageBounds);

                var voltageY = bestMatchingY - TextPadding;
                canvas.DrawText(voltageString, bordersRect.Left + TextPadding, voltageY, textPaint); // Left
                canvas.DrawText(voltageString, bordersRect.Right - voltageBounds.Width - TextPadding, voltageY, textPaint); // Right

                // Matcher position (at vertical line)
                var matcherString = $"{ ViewModel.MainModel.ArmingModel.BestMatchingPosition }";

                var matcherBounds = new SKRect();
                textPaint.MeasureText(matcherString, ref matcherBounds);

                var matcherX = bestMatchingX + TextPadding;
                canvas.DrawText(matcherString, matcherX, bordersRect.Top + matcherBounds.Height + TextPadding, textPaint); // Top
                canvas.DrawText(matcherString, matcherX, bordersRect.Bottom - TextPadding, textPaint); // Bottom

            }
        }

        private float CalculateCellWidth(SKRect borders)
        {
            return borders.Width / (NumberOfMatchingPositions - 1);
        }

        private float MatcherPositionToX(SKRect borders, int position)
        {
            return borders.Left + CalculateCellWidth(borders) * position;
        }

        private float AntennaVoltageToY(SKRect borders, float yScale, float voltage)
        {
            return borders.Bottom - voltage * yScale;
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

        private async Task OnAntennaMatchingMeasurementAsync(IAntennaMatchingMeasurementEvent antennaMatchingMeasurementEvent)
        {
            MatchingGraphView.InvalidateSurface();
        }

        private async Task OnFoxDisarmedAsync(IFoxDisarmedEvent foxDisarmedEvent)
        {
            await Navigation.PopModalAsync();
        }
    }
}