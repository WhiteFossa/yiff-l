﻿using org.whitefossa.yiffhl.Abstractions.Enums;
using org.whitefossa.yiffhl.Abstractions.Interfaces;
using org.whitefossa.yiffhl.Abstractions.Interfaces.Events;
using org.whitefossa.yiffhl.ViewModels;
using System;
using System.Threading.Tasks;
using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace org.whitefossa.yiffhl.Views
{
    [XamlCompilation(XamlCompilationOptions.Compile)]
    public partial class MainPageView : ContentPage
    {
        MainPageViewModel ViewModel
        {
            get => BindingContext as MainPageViewModel;
            set => BindingContext = value;
        }

        public MainPageView()
        {
            InitializeComponent();
            ViewModel.Navigation = Navigation;
        }

        private void pkFox_SelectedIndexChanged(object sender, EventArgs e)
        {
            var selectedFox = (sender as Picker).SelectedItem;

            ViewModel.SelectedFoxChangedCommand.Execute(selectedFox);
        }

        private void pkProfile_SelectedIndexChanged(object sender, EventArgs e)
        {
            var selectedProfile = (sender as Picker).SelectedItem;

            ViewModel.SelectedProfileChangedCommand.Execute(selectedProfile);
        }

        private void pkCallsign_SelectedIndexChanged(object sender, EventArgs e)
        {
            var selectedCallsign = (sender as Picker).SelectedItem;

            ViewModel.SelectedCallsignChangedCommand.Execute(selectedCallsign);
        }

        private void tpStart_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName != "Time")
            {
                return;
            }

            var startTime = (sender as TimePicker).Time;
            ViewModel.SetStartTimeCommand.Execute(startTime);
        }

        private void tpFinish_PropertyChanged(object sender, System.ComponentModel.PropertyChangedEventArgs e)
        {
            if (e.PropertyName != "Time")
            {
                return;
            }

            var finishTime = (sender as TimePicker).Time;
            ViewModel.SetFinishTimeCommand.Execute(finishTime);
        }
    }
}