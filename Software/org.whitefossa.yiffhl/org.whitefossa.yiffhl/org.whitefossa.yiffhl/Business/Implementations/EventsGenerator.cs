using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Models;
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;

namespace org.whitefossa.yiffhl.Business.Implementations
{
    public class EventsGenerator : IEventsGenerator
    {
        private OnShowMatchingDisplayDelegate _onShowMatchingDisplay;
        private OnFoxArmedDelegate _onFoxArmed;

        private bool? _isFoxArmedOld = null;

        public async Task GenerateEventsAsync(MainModel model)
        {
            if (!_isFoxArmedOld.HasValue)
            {
                _isFoxArmedOld = model.DynamicFoxStatus.IsFoxArmed;
            }

            #region Show matching display

            if
            (
                model.ActiveDisplay != ActiveDisplay.MatchingDisplay
                &&
                (
                    model.DynamicFoxStatus.AntennaMatchingStatus.Status == AntennaMatchingStatus.InProgress
                    ||
                    model.DynamicFoxStatus.AntennaMatchingStatus.Status == AntennaMatchingStatus.Completed
                )
                &&
                model.DynamicFoxStatus.AntennaMatchingStatus.IsNewForApp
            )
            {
                // Showing matching display
                if (_onShowMatchingDisplay != null)
                {
                    _onShowMatchingDisplay();
                }
            }

            #endregion

            #region Fox is armed

            if (model.DynamicFoxStatus.IsFoxArmed && !_isFoxArmedOld.Value)
            {
                if (_onFoxArmed != null)
                {
                    _onFoxArmed();
                }
            }

            #endregion

            _isFoxArmedOld = model.DynamicFoxStatus.IsFoxArmed;
        }

        public void RegisterOnFoxArmedHandler(OnFoxArmedDelegate onFoxArmed)
        {
            _onFoxArmed = onFoxArmed ?? throw new ArgumentNullException(nameof(onFoxArmed));
        }

        public void RegisterOnShowMatchingDisplayHandler(OnShowMatchingDisplayDelegate onShowMatchingDisplay)
        {
            _onShowMatchingDisplay = onShowMatchingDisplay ?? throw new ArgumentNullException(nameof(onShowMatchingDisplay));
        }
    }
}
