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
        private OnFoxDisarmedDelegate _onFoxDisarmed;
        private OnProfileChangedDelegate _onProfileChanged;
        private OnProfileSettingsChangedDelegate _onProfileSettingsChanged;

        private bool? _isFoxArmedOld = null;
        private int? _currentProfileIdOld = null;

        public async Task GenerateEventsAsync(MainModel model)
        {
            if (!_isFoxArmedOld.HasValue)
            {
                _isFoxArmedOld = model.DynamicFoxStatus.IsFoxArmed;
            }

            if (!_currentProfileIdOld.HasValue)
            {
                _currentProfileIdOld = model.DynamicFoxStatus.CurrentProfileId;
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

            #region Fox is disarmed

            if (!model.DynamicFoxStatus.IsFoxArmed && _isFoxArmedOld.Value)
            {
                if (_onFoxDisarmed != null)
                {
                    _onFoxDisarmed();
                }
            }

            #endregion

            #region Profile changed

            if (model.DynamicFoxStatus.CurrentProfileId != _currentProfileIdOld.Value)
            {
                if (_onProfileChanged != null)
                {
                    _onProfileChanged(model.DynamicFoxStatus.CurrentProfileId);
                }
            }

            #endregion

            #region Profile settings changed

            if (model.DynamicFoxStatus.IsManualProfileChangesExist)
            {
                if (_onProfileSettingsChanged != null)
                {
                    _onProfileSettingsChanged();
                }
            }

            #endregion

            _isFoxArmedOld = model.DynamicFoxStatus.IsFoxArmed;
            _currentProfileIdOld = model.DynamicFoxStatus.CurrentProfileId;
        }

        public void RegisterOnFoxArmedHandler(OnFoxArmedDelegate onFoxArmed)
        {
            _onFoxArmed = onFoxArmed ?? throw new ArgumentNullException(nameof(onFoxArmed));
        }

        public void RegisterOnFoxDisarmedHandler(OnFoxDisarmedDelegate onFoxDisarmed)
        {
            _onFoxDisarmed = onFoxDisarmed ?? throw new ArgumentNullException(nameof(onFoxDisarmed));
        }

        public void RegisterOnProfileChangedHandler(OnProfileChangedDelegate onProfileChanged)
        {
            _onProfileChanged = onProfileChanged ?? throw new ArgumentNullException(nameof(onProfileChanged));
        }

        public void RegisterOnProfileSettingsChangedHandler(OnProfileSettingsChangedDelegate onProfileSettingsChanged)
        {
            _onProfileSettingsChanged = onProfileSettingsChanged ?? throw new ArgumentNullException(nameof(onProfileSettingsChanged));
        }

        public void RegisterOnShowMatchingDisplayHandler(OnShowMatchingDisplayDelegate onShowMatchingDisplay)
        {
            _onShowMatchingDisplay = onShowMatchingDisplay ?? throw new ArgumentNullException(nameof(onShowMatchingDisplay));
        }
    }
}
