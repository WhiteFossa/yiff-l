﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using yiff_hl.Abstractions.Enums;
using yiff_hl.Abstractions.Interfaces;
using yiff_hl.Business.Implementations.Commands.Helpers;

namespace yiff_hl.Business.Implementations.Commands
{
    public delegate void OnSetFoxNameResponseDelegate(bool isSuccessfull);

    public class SetFoxNameCommand
    {
        private const int MinNameLength = 1;
        private const int MaxNameLength = 16;

        private readonly IPacketsProcessor packetsProcessor;
        private OnSetFoxNameResponseDelegate onSetFoxNameResponse;

        private SetFoxNameCommand()
        {

        }

        public SetFoxNameCommand(IPacketsProcessor packetsProcessor)
        {
            this.packetsProcessor = packetsProcessor ?? throw new ArgumentNullException(nameof(packetsProcessor));
            packetsProcessor.SetOnSetFoxNameResponse(OnSetFoxNameResponse);
        }

        public void SetResponseDelegate(OnSetFoxNameResponseDelegate onSetFoxNameResponse)
        {
            this.onSetFoxNameResponse = onSetFoxNameResponse;
        }

        public void SendSetFoxNameCommand(string name)
        {
            if (name.Length < MinNameLength || name.Length > MaxNameLength)
            {
                throw new ArgumentException("Invalid name length", nameof(name));
            }

            var payload = new List<byte>();

            // 2th (from 0th) byte - name length
            payload.Add((byte)(name.Length));

            // 3th - 35th bytes - name
            var nameBytes = Encoding.ASCII.GetBytes(name);
            payload.AddRange(nameBytes);

            packetsProcessor.SendCommand(CommandType.SetFoxName, payload);
        }

        private void OnSetFoxNameResponse(IReadOnlyCollection<byte> payload)
        {
            if (onSetFoxNameResponse == null)
            {
                return;
            }

            if (payload.Count != 1)
            {
                return;
            }

            onSetFoxNameResponse(CommandsHelper.IsSuccessful(payload.ElementAt(0)));
        }
    }
}