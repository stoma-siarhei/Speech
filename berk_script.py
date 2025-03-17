from bark import SAMPLE_RATE, generate_audio, preload_models
from scipy.io.wavfile import write as write_wav
from IPython.display import Audio

def berk_func(str_wave, file_name):
    # download and load all models
    preload_models()

    # generate audio from text
    audio_array = generate_audio(str_wave)

    # save audio to disk
    write_wav(file_name, SAMPLE_RATE, audio_array)
      
    # play text in notebook
    Audio(audio_array, rate=SAMPLE_RATE)
