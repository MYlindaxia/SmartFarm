param(
    [Parameter(Mandatory = $true)][string]$ImagePath,
    [string]$LangTag = "",
    [switch]$Boxes
)

Add-Type -AssemblyName System.Runtime.WindowsRuntime -ErrorAction Stop | Out-Null

$asTaskGeneric = ([System.WindowsRuntimeSystemExtensions].GetMethods() |
    Where-Object {
        $_.Name -eq 'AsTask' -and
        $_.GetParameters().Count -eq 1 -and
        $_.GetParameters()[0].ParameterType.Name -eq 'IAsyncOperation`1'
    })[0]

function Await($WinRtTask, $ResultType) {
    $asTask = $asTaskGeneric.MakeGenericMethod($ResultType)
    $netTask = $asTask.Invoke($null, @($WinRtTask))
    $netTask.Wait(-1) | Out-Null
    $netTask.Result
}

[Windows.Storage.StorageFile, Windows.Storage, ContentType = WindowsRuntime] | Out-Null
[Windows.Graphics.Imaging.BitmapDecoder, Windows.Graphics.Imaging, ContentType = WindowsRuntime] | Out-Null
[Windows.Media.Ocr.OcrEngine, Windows.Media.Ocr, ContentType = WindowsRuntime] | Out-Null
[Windows.Globalization.Language, Windows.Globalization, ContentType = WindowsRuntime] | Out-Null

$engine = $null
if ($LangTag -ne "") {
    $lang = New-Object Windows.Globalization.Language($LangTag)
    $engine = [Windows.Media.Ocr.OcrEngine]::TryCreateFromLanguage($lang)
}
if ($null -eq $engine) {
    $engine = [Windows.Media.Ocr.OcrEngine]::TryCreateFromUserProfileLanguages()
}
if ($null -eq $engine) {
    Write-Error "No OCR engine available."
    exit 1
}

[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
Write-Output ("ENGINE: " + $engine.RecognizerLanguage.LanguageTag)

$file = Await ([Windows.Storage.StorageFile]::GetFileFromPathAsync($ImagePath)) ([Windows.Storage.StorageFile])
$stream = Await ($file.OpenAsync([Windows.Storage.FileAccessMode]::Read)) ([Windows.Storage.Streams.IRandomAccessStream])
$decoder = Await ([Windows.Graphics.Imaging.BitmapDecoder]::CreateAsync($stream)) ([Windows.Graphics.Imaging.BitmapDecoder])
$bitmap = Await ($decoder.GetSoftwareBitmapAsync()) ([Windows.Graphics.Imaging.SoftwareBitmap])
$result = Await ($engine.RecognizeAsync($bitmap)) ([Windows.Media.Ocr.OcrResult])

foreach ($line in $result.Lines) {
    if ($Boxes) {
        $words = @($line.Words)
        $x = [int]$words[0].BoundingRect.X
        $y = [int]$words[0].BoundingRect.Y
        $w = [int]$words[$words.Count - 1].BoundingRect.Right
        $h = [int]$words[$words.Count - 1].BoundingRect.Bottom
        Write-Output ("[{0},{1} - {2},{3}] {4}" -f $x, $y, $w, $h, $line.Text)
    }
    else {
        Write-Output $line.Text
    }
}
