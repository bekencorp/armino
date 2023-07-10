void audio_init(void);
void audio_deinit(void);

void voice_init(uint8_t type, aud_intf_voc_samp_rate_t samp_rate, bool aec_en);
void voice_start(void);
void voice_stop(void);
void voice_deinit(void);
void voice_spk_ctrl(bool enable);
void voice_mic_ctrl(bool enable);
