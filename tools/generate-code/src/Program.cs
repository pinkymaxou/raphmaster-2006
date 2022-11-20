﻿using System;
using System.Diagnostics;
using System.IO;

namespace generate_code
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                Console.Error.WriteLine("Generate settings matrix");

                int slotCount = 16;

                Func<int, string> enumPosXText       = (i) => $"SETTINGS_EENTRY_SLOT{i}_POS_X";
                Func<int, string> enumPosYText       = (i) => $"SETTINGS_EENTRY_SLOT{i}_POS_Y";
                Func<int, string> enumLoadedIDText   = (i) => $"SETTINGS_EENTRY_SLOT{i}_LOADID";
                
                Func<int, string> enumTotalQtyText = (i) => $"SETTINGS_EENTRY_SLOT{i}_TOTALQTY";
                Func<int, string> enumUsedQtyText = (i) => $"SETTINGS_EENTRY_SLOT{i}_USEDQTY";

                string settingsMatrix1Txt = "settingmatrix1.txt";

                using (StreamWriter fs = new StreamWriter(settingsMatrix1Txt, false, System.Text.Encoding.UTF8))
                {
                    for (int i = 1; i <= slotCount; i++)
                    {
                        fs.WriteLine($"{ enumPosXText(i) },");
                        fs.WriteLine($"{ enumPosYText(i) },");
                        fs.WriteLine($"{ enumLoadedIDText(i) },");
                        fs.WriteLine($"{ enumTotalQtyText(i) },");
                        fs.WriteLine($"{ enumUsedQtyText(i) },");
                        fs.WriteLine();
                    }
                }

                string settingsMatrix2Txt = "settingmatrix2.txt";

                using (StreamWriter fs = new StreamWriter(settingsMatrix2Txt, false, System.Text.Encoding.UTF8))
                {
                    for (int i = 1; i <= slotCount; i++)
                    {
                        fs.WriteLine($"[{enumPosXText(i)}] = NVSJSON_INITINT32_RNG(\"S{i}.PosX\", \"Position X\", 0, 0, 100000, NVSJSON_EFLAGS_None),");
                        fs.WriteLine($"[{enumPosYText(i)}] = NVSJSON_INITINT32_RNG(\"S{i}.PosY\", \"Position Y\", 0, 0, 100000, NVSJSON_EFLAGS_None),");
                        fs.WriteLine($"[{enumLoadedIDText(i)}] = NVSJSON_INITINT32_RNG(\"S{i}.LoadID\", \"Ingredient ID, 0 = nothing\", 0, 0, 100000, NVSJSON_EFLAGS_None),");

                        fs.WriteLine($"[{enumTotalQtyText(i)}] = NVSJSON_INITINT32_RNG(\"S{i}.TotalML\", \"Total Qty (ml)\", 0, 0, 6000, NVSJSON_EFLAGS_None),");
                        fs.WriteLine($"[{enumUsedQtyText(i)}] = NVSJSON_INITINT32_RNG(\"S{i}.UsedML\", \"Used Qty (ml)\", 0, 0, 6000, NVSJSON_EFLAGS_None),");
                        fs.WriteLine();
                    }
                }

                foreach (string filename in new string[] { settingsMatrix1Txt, settingsMatrix2Txt })
                {
                    Process.Start(new ProcessStartInfo()
                    {
                        FileName = filename,
                        UseShellExecute = true
                    }); ;
                }
            }
            catch (Exception ex)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.Error.WriteLine("Error: " + ex.Message);
            }
        }
    }
}