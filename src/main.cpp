#include <iostream>
#include <unistd.h>
#include "../inc/AudioFFT.h"
#include "../inc/Utilities.h"
#include "../inc/FFTConvolver.h"
#include "../inc/wav.h"

struct waveInfo {
    int channel_count; // Channel count
    long sample_rate;  // Sample rate
    long sample_count; // Sample count
};

int main() {
    setbuf(stdout,0);
    struct waveInfo inputInfo, impulseInfo;
    float *impulseBuffer, *inputBuffer, *outputBuffer;


    ///READ IN INPUT
    WavFile *inputFile = wav_open("..\\sound\\inputs\\hello_tom.wav",WAV_OPEN_READ);
    inputInfo.channel_count = (int)wav_get_num_channels(inputFile);
    inputInfo.sample_rate = (long)wav_get_sample_rate(inputFile);
    inputInfo.sample_count = (long)wav_get_length(inputFile);

    ///READ IN IMPULSE
    WavFile *impulseFile = wav_open("..\\sound\\impulses\\impulse.wav",WAV_OPEN_READ);
    impulseInfo.channel_count = (int)wav_get_num_channels(impulseFile);
    impulseInfo.sample_rate = (long)wav_get_sample_rate(impulseFile);
    impulseInfo.sample_count = (long)wav_get_length(impulseFile);

    ///CREATE OUTPUT FILE
    WavFile *outputFile = wav_open("..\\sound\\output.wav", WAV_OPEN_WRITE);
    wav_set_format(outputFile, WAV_FORMAT_IEEE_FLOAT); // Set wav format to FLOAT
    wav_set_num_channels(outputFile, (WavU16)inputInfo.channel_count); // Set number of channels to same as input
    wav_set_sample_rate(outputFile, (WavU32)inputInfo.sample_rate);

    ///ALLOCATE MEM FOR BUFFERS
    impulseBuffer = static_cast<float *>(malloc(sizeof(float) * (unsigned long) impulseInfo.sample_count * impulseInfo.channel_count)); //Allocate enough memory for both channels if stereo
    //memset(inputBuffer, 0, (sizeof(float) * ((unsigned long)inputInfo.sample_count * 2) - 1));
    inputBuffer = static_cast<float *>(malloc(sizeof(float) * (unsigned long) inputInfo.sample_count * inputInfo.channel_count));
    //memset(impulseBuffer, 0, (sizeof(float) * ((unsigned long)impulseInfo.sample_count * 2) - 1));

    outputBuffer = static_cast<float *>(malloc(sizeof(float) * (unsigned long) inputInfo.sample_count * inputInfo.channel_count));
    //memset(outputBuffer, 0, (sizeof(float) * ((unsigned long)inputInfo.sample_count * 2) - 1));

    ///READ IN FILES
    wav_read(inputFile, inputBuffer, (size_t)inputInfo.sample_count); // Read files into buffers
    wav_read(impulseFile,impulseBuffer,(size_t)impulseInfo.sample_count);

    ///CONVOLVE
    auto convolve = new fftconvolver::FFTConvolver(); //Create convolution object

    convolve->init(impulseInfo.sample_count,impulseBuffer,impulseInfo.sample_count); //Initialize the object with impulse buffer

    convolve->process(inputBuffer,outputBuffer,inputInfo.sample_count + (impulseInfo.sample_count/2)); // process the input with the conv. Last argument is arbitrary, but needed more than input and less than input+impulse samples count

    wav_write(outputFile,outputBuffer,(size_t)inputInfo.sample_count); //write to new wav file

    ///FREE & CLOSE
    free(impulseBuffer);
    free(inputBuffer);
    free(outputBuffer);
    wav_close(impulseFile);
    wav_close(inputFile);
    wav_close(outputFile);

    return 0;
}
