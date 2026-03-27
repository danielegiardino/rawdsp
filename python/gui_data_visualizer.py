import tkinter as tk
from tkinter import filedialog, messagebox, ttk
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from scipy import signal
import os

class DataVisualizerApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Binary Data Visualizer - IQ & Filter Analysis Tool")
        self.root.geometry("1300x1050")
        self.root.protocol("WM_DELETE_WINDOW", self.on_closing)

        self.processed_data = None
        self.file_path = ""
        self.file_name = "No file selected"
        self.const_win = None
        self.spec_win = None

        self.setup_ui()
        self.bind_shortcuts()

    def on_closing(self):
        plt.close('all')
        self.root.quit()
        self.root.destroy()

    def bind_shortcuts(self):
        self.root.bind('<Control-o>', lambda e: self.browse_file())
        self.root.bind('<Control-u>', lambda e: self.run_process())

    def safe_eval(self, expression):
        try:
            clean_expr = str(expression).replace('^', '**').strip()
            return float(eval(clean_expr, {"__builtins__": None}, {}))
        except: return None

    def setup_ui(self):
        info_frame = ttk.Frame(self.root)
        info_frame.pack(fill="x", padx=10, pady=(5, 0))
        self.lbl_filename = ttk.Label(info_frame, text=f"File: {self.file_name}", font=("TkDefaultFont", 10, "bold"))
        self.lbl_filename.pack(side="top", anchor="w")
        self.lbl_path = ttk.Label(info_frame, text="Path: -", font=("TkDefaultFont", 8), foreground="gray")
        self.lbl_path.pack(side="top", anchor="w")

        config_frame = ttk.LabelFrame(self.root, text="Settings & Loading Controls")
        config_frame.pack(fill="x", padx=10, pady=5)

        # Row 0: Loading Params
        ttk.Label(config_frame, text="File Offset:").grid(row=0, column=0, padx=5, sticky="e")
        self.load_offset = tk.StringVar(value="0")
        ttk.Entry(config_frame, textvariable=self.load_offset, width=10).grid(row=0, column=1, sticky="w")
        
        ttk.Label(config_frame, text="Num Samples:").grid(row=0, column=2, padx=5, sticky="e")
        self.load_count = tk.StringVar(value="10^5") 
        ttk.Entry(config_frame, textvariable=self.load_count, width=10).grid(row=0, column=3, sticky="w")

        # Row 1: Data Config
        ttk.Label(config_frame, text="Data Type:").grid(row=1, column=0, padx=5, pady=5, sticky="e")
        self.dtype_var = tk.StringVar(value="int16")
        ttk.Combobox(config_frame, textvariable=self.dtype_var, values=["int16", "int32", "float32", "float64", "long double"], width=12, state="readonly").grid(row=1, column=1, sticky="w")
        
        self.is_complex = tk.BooleanVar(value=True); ttk.Checkbutton(config_frame, text="Complex (I/Q)", variable=self.is_complex).grid(row=1, column=2)
        self.swap_iq = tk.BooleanVar(value=False); ttk.Checkbutton(config_frame, text="Swap I/Q", variable=self.swap_iq).grid(row=1, column=3)
        self.use_markers = tk.BooleanVar(value=True); ttk.Checkbutton(config_frame, text="Show Markers", variable=self.use_markers).grid(row=1, column=4)
        
        # Row 2: Frequency Plot Logic
        ttk.Label(config_frame, text="Fs (Hz):").grid(row=2, column=0, padx=5, sticky="e")
        self.sample_rate = tk.StringVar(value="10^6"); ttk.Entry(config_frame, textvariable=self.sample_rate, width=12).grid(row=2, column=1, sticky="w")
        
        ttk.Label(config_frame, text="Plot Mode:").grid(row=2, column=2, sticky="e")
        self.freq_mode = tk.StringVar(value="Magnitude (FFT)")
        ttk.Combobox(config_frame, textvariable=self.freq_mode, values=["PSD (Welch)", "Magnitude (FFT)"], width=15, state="readonly").grid(row=2, column=3, sticky="w")
        
        self.normalize_0db = tk.BooleanVar(value=True)
        ttk.Checkbutton(config_frame, text="Normalize 0dB", variable=self.normalize_0db).grid(row=2, column=4)

        # Row 3: FFT and Display Limits
        ttk.Label(config_frame, text="NFFT:").grid(row=3, column=0, sticky="e")
        self.nfft_val = tk.StringVar(value="1024")
        ttk.Entry(config_frame, textvariable=self.nfft_val, width=12).grid(row=3, column=1, sticky="w")

        ttk.Label(config_frame, text="Time Plot Samples:").grid(row=3, column=2, padx=5, sticky="e")
        self.time_plot_limit = tk.StringVar(value="1000")
        ttk.Entry(config_frame, textvariable=self.time_plot_limit, width=10).grid(row=3, column=3, sticky="w")

        # Buttons
        btn_row = ttk.Frame(config_frame); btn_row.grid(row=4, column=0, columnspan=5, pady=5, sticky="w")
        ttk.Button(btn_row, text="Open Constellation", command=self.open_constellation_window).pack(side="left", padx=5)
        ttk.Button(btn_row, text="Open Spectrogram", command=self.open_spectrogram_window).pack(side="left", padx=5)
        self.axis_mode = tk.StringVar(value="Sample Index [n]")
        ttk.Combobox(btn_row, textvariable=self.axis_mode, values=["Time [s]", "Sample Index [n]"], width=15, state="readonly").pack(side="left", padx=5)

        btn_container = ttk.Frame(config_frame); btn_container.grid(row=0, column=6, rowspan=5, padx=20, sticky="ns")
        ttk.Button(btn_container, text="Browse File", command=self.browse_file).pack(pady=5, fill="x")
        ttk.Button(btn_container, text="Update All (Ctrl+U)", command=self.run_process).pack(pady=5, fill="x")

        # --- SIGNAL ANALYSIS PANEL ---
        self.info_panel = ttk.LabelFrame(self.root, text="SIGNAL ANALYSIS")
        self.info_panel.pack(fill="x", padx=10, pady=2)
        self.lbl_rms = ttk.Label(self.info_panel, text="RMS: -")
        self.lbl_papr = ttk.Label(self.info_panel, text="PAPR: -")
        self.lbl_snr = ttk.Label(self.info_panel, text="Est. SNR: -")
        self.lbl_floor_diff = ttk.Label(self.info_panel, text="Signal/Noise (dB): -")
        self.lbl_dc = ttk.Label(self.info_panel, text="DC (I/Q): -")
        for lbl in [self.lbl_rms, self.lbl_papr, self.lbl_snr, self.lbl_floor_diff, self.lbl_dc]: lbl.pack(side="left", padx=15)

        self.toolbar_frame = ttk.Frame(self.root); self.toolbar_frame.pack(fill="x", padx=10, pady=2)
        self.plot_container = ttk.Frame(self.root); self.plot_container.pack(fill="both", expand=True, padx=10, pady=5)
        self.canvas = None; self.toolbar = None
        self.create_plots(2)

    def open_constellation_window(self):
        if self.processed_data is None: return
        self.const_win = ConstellationWindow(self.root, self.processed_data, self.file_name)

    def open_spectrogram_window(self):
        if self.processed_data is None: return
        self.spec_win = SpectrogramWindow(self.root, self.processed_data, self.safe_eval(self.sample_rate.get()))

    def create_plots(self, n_plots):
        if self.canvas: self.canvas.get_tk_widget().destroy()
        if self.toolbar: self.toolbar.destroy()
        self.fig, self.axs = plt.subplots(n_plots, 1, figsize=(8, 10), constrained_layout=True)
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.plot_container)
        self.canvas.get_tk_widget().pack(fill="both", expand=True)
        self.toolbar = NavigationToolbar2Tk(self.canvas, self.toolbar_frame); self.toolbar.update()

    def browse_file(self):
        path = filedialog.askopenfilename(parent=self.root, title="Select Binary IQ Data File",
                                          filetypes=[('Binary Data', '*.dat *.bin'), ('All Files', '*.*')])
        if path:
            self.file_path = path; self.file_name = os.path.basename(path)
            self.lbl_filename.config(text=f"File: {self.file_name}")
            self.lbl_path.config(text=f"Path: {os.path.dirname(path)}")
            self.run_process()

    def run_process(self):
        if not self.file_path: return
        fs = self.safe_eval(self.sample_rate.get())
        nfft = int(self.safe_eval(self.nfft_val.get()) or 1024)
        try:
            dt_map = {
                "int16": np.int16, "int32": np.int32, "float32": np.float32, 
                "float64": np.float64, "long double": np.longdouble
            }
            dtype = dt_map[self.dtype_var.get()]
            bytes_per_sample = np.dtype(dtype).itemsize * (2 if self.is_complex.get() else 1)
            offset, count = int(self.safe_eval(self.load_offset.get())), int(self.safe_eval(self.load_count.get()))
            
            with open(self.file_path, 'rb') as f:
                f.seek(offset * bytes_per_sample)
                read_count = -1 if count == 0 else (count * 2 if self.is_complex.get() else count)
                data = np.fromfile(f, dtype=dtype, count=read_count)

            if self.is_complex.get():
                if len(data) % 2 != 0: data = data[:-1]
                parts = data.reshape(-1, 2)
                data = (parts[:, 1] + 1j * parts[:, 0]) if self.swap_iq.get() else (parts[:, 0] + 1j * parts[:, 1])
            
            self.processed_data = data
            self.update_signal_info(data, fs, nfft)
            self.update_plots(fs, nfft)
            
            if self.const_win and tk.Toplevel.winfo_exists(self.const_win):
                if not self.const_win.is_frozen.get():
                    self.const_win.data = data; self.const_win.update()
            if self.spec_win and tk.Toplevel.winfo_exists(self.spec_win):
                if not self.spec_win.is_frozen.get():
                    self.spec_win.data = data; self.spec_win.fs = fs; self.spec_win.plot_spec()
        except Exception as e: messagebox.showerror("Error", f"Loading failed: {e}")

    def update_signal_info(self, data, fs, nfft):
        mag = np.abs(data); rms = np.sqrt(np.mean(mag**2))
        papr = 20 * np.log10(np.max(mag) / rms) if rms > 0 else 0
        dc_i, dc_q = np.mean(data.real), np.mean(data.imag)
        m2, m4 = np.mean(mag**2), np.mean(mag**4)
        snr_db = 10 * np.log10(np.sqrt(2*m2**2 - m4) / (m2 - np.sqrt(2*m2**2 - m4))) if (2*m2**2 - m4) > 0 else 0
        f, Pxx = signal.periodogram(data, fs, nfft=nfft, return_onesided=False)
        psd_db = 10 * np.log10(Pxx + 1e-18)
        n_db = np.max(psd_db) - np.median(psd_db)

        self.lbl_rms.config(text=f"RMS: {rms:.4f}")
        self.lbl_papr.config(text=f"PAPR: {papr:.2f} dB")
        self.lbl_snr.config(text=f"Est. SNR: {snr_db:.2f} dB")
        self.lbl_floor_diff.config(text=f"Signal/Noise: {n_db:.2f} dB")
        self.lbl_dc.config(text=f"DC (I/Q): {dc_i:.4f} / {dc_q:.4f}")

    def update_plots(self, fs, nfft):
        data = self.processed_data; marker = 's' if self.use_markers.get() else None
        ax0 = self.axs[0]; ax0.clear()
        
        plot_limit = int(self.safe_eval(self.time_plot_limit.get()) or 1000)
        limit = min(len(data), plot_limit)
        x = (np.arange(limit) / fs) if self.axis_mode.get() == "Time [s]" else np.arange(limit)
        
        ax0.step(x, data.real[:limit], where='post', label="I", marker=marker, markersize=3)
        if (self.is_complex.get()):
            ax0.step(x, data.imag[:limit], where='post', label="Q", marker=marker, markersize=3, alpha=0.6)
        ax0.set_title("Time Domain"); ax0.legend(); ax0.grid(True, alpha=0.3)
        
        ax1 = self.axs[1]; ax1.clear()
        
        # --- IMPROVED FREQUENCY PLOTTING ---
        if self.freq_mode.get() == "PSD (Welch)":
            f, Pxx = signal.welch(data, fs, nperseg=nfft, return_onesided=False)
            y_plot = 10 * np.log10(Pxx + 1e-18)
        else: # Magnitude (FFT) Mode - Best for FIR Filters
            # Use the data block loaded, zero-pad to nfft
            H = np.fft.fft(data[:nfft] if len(data) >= nfft else data, nfft)
            f = np.fft.fftfreq(nfft, 1/fs)
            y_plot = 20 * np.log10(np.abs(H) + 1e-18)
            
        f, y_plot = np.fft.fftshift(f), np.fft.fftshift(y_plot)
        
        # Apply 0dB Normalization if requested
        if self.normalize_0db.get():
            y_plot -= np.max(y_plot)
            
        ax1.plot(f, y_plot, color='red')
        ax1.set_title(f"Frequency Domain ({self.freq_mode.get()})")
        ax1.set_ylabel("dB" if not self.normalize_0db.get() else "dB (Normalized)")
        ax1.set_xlabel("Frequency [Hz]")
        ax1.grid(True, alpha=0.3)
        self.canvas.draw()

class ConstellationWindow(tk.Toplevel):
    def __init__(self, parent, data, fname):
        super().__init__(parent)
        self.title(f"Constellation - {fname}"); self.geometry("1150x1050"); self.data = data
        self.nav_frame = ttk.Frame(self); self.nav_frame.pack(fill="x", side="top", padx=10)
        ctrl = ttk.LabelFrame(self, text="Controls"); ctrl.pack(fill="x", padx=10, pady=5)
        
        row1 = ttk.Frame(ctrl); row1.pack(fill="x", padx=5, pady=2)
        self.is_frozen = tk.BooleanVar(value=False); ttk.Checkbutton(row1, text="FREEZE", variable=self.is_frozen).pack(side="left")
        ttk.Label(row1, text="SPS:").pack(side="left"); self.sps_val = tk.StringVar(value="1"); ttk.Entry(row1, textvariable=self.sps_val, width=5).pack(side="left")
        ttk.Label(row1, text="Off:").pack(side="left"); self.ds_offset = tk.StringVar(value="0"); ttk.Entry(row1, textvariable=self.ds_offset, width=5).pack(side="left")
        self.use_density = tk.BooleanVar(value=False); ttk.Checkbutton(row1, text="Density", variable=self.use_density).pack(side="left", padx=10)
        self.mod_type = tk.StringVar(value="None"); ttk.OptionMenu(row1, self.mod_type, "None", "None", "BPSK", "QPSK", "8-PSK", "16-QAM", "64-QAM").pack(side="left")
        ttk.Button(row1, text="Apply All", command=self.update).pack(side="right", padx=10)

        for label, var_name, default in [("Gain:", "sig_scale", "1.0"), ("Phase:", "phase_deg", "0.0"), ("Ref Gain:", "ref_scale", "1.0")]:
            row = ttk.Frame(ctrl); row.pack(fill="x", pady=2)
            ttk.Label(row, text=label, width=10).pack(side="left")
            var = tk.DoubleVar(value=float(default)); setattr(self, var_name, var)
            mx = 180 if "Phase" in label else 100
            tk.Scale(row, variable=var, from_=-mx if "Phase" in label else 0, to=mx, resolution=0.01, orient="horizontal", length=400, showvalue=0, command=lambda v, n=var_name: self._sync(n)).pack(side="left")
            entry = ttk.Entry(row, width=8); entry.insert(0, default); entry.pack(side="left", padx=5)
            setattr(self, f"{var_name}_entry", entry); entry.bind('<Return>', lambda e, n=var_name: self._sync_entry(n))

        range_frame = ttk.Frame(ctrl); range_frame.pack(fill="x", pady=5)
        self.start_idx = tk.IntVar(value=0); self.end_idx = tk.IntVar(value=5000)
        
        s_row = ttk.Frame(range_frame); s_row.pack(fill="x")
        ttk.Label(s_row, text="Start Sample:", width=12).pack(side="left")
        self.start_scale = tk.Scale(s_row, variable=self.start_idx, from_=0, to=100000, orient="horizontal", length=400, showvalue=0, command=lambda v: self.update())
        self.start_scale.pack(side="left"); self.lbl_start = ttk.Label(s_row, text="0"); self.lbl_start.pack(side="left", padx=5)
        
        e_row = ttk.Frame(range_frame); e_row.pack(fill="x")
        ttk.Label(e_row, text="End Sample:", width=12).pack(side="left")
        self.end_scale = tk.Scale(e_row, variable=self.end_idx, from_=0, to=100000, orient="horizontal", length=400, showvalue=0, command=lambda v: self.update())
        self.end_scale.pack(side="left"); self.lbl_end = ttk.Label(e_row, text="5000"); self.lbl_end.pack(side="left", padx=5)

        self.fig, self.ax = plt.subplots(figsize=(6,6))
        self.canvas = FigureCanvasTkAgg(self.fig, master=self); self.canvas.get_tk_widget().pack(fill="both", expand=True)
        self.toolbar = NavigationToolbar2Tk(self.canvas, self.nav_frame); self.toolbar.update(); self.update()

    def _sync(self, name):
        e = getattr(self, f"{name}_entry"); e.delete(0, tk.END); e.insert(0, f"{getattr(self, name).get():.2f}"); self.update()
    def _sync_entry(self, name):
        try: getattr(self, name).set(float(getattr(self, f"{name}_entry").get())); self.update()
        except: pass

    def update(self, _=None):
        try:
            sampled = self.data[int(self.ds_offset.get())::int(self.sps_val.get())]
            data_len = len(sampled)
            self.start_scale.config(to=data_len); self.end_scale.config(to=data_len)
            start, end = self.start_idx.get(), self.end_idx.get()
            self.lbl_start.config(text=str(start)); self.lbl_end.config(text=str(end))
            chunk = sampled[start:end] * self.sig_scale.get() * np.exp(1j * np.deg2rad(self.phase_deg.get()))
            self.ax.clear()
            if self.use_density.get(): self.ax.hist2d(chunk.real, chunk.imag, bins=100, cmap='viridis')
            else: self.ax.scatter(chunk.real, chunk.imag, s=4, alpha=0.5)
            
            mod = self.mod_type.get()
            ref_pts = []
            if mod == "BPSK": ref_pts = [np.exp(1j * 2 * np.pi * n / 2) for n in range(2)]
            elif mod == "QPSK": ref_pts = [np.exp(1j * 2 * np.pi * n / 4 + 1j * np.pi/4) for n in range(4)]
            elif mod == "8-PSK": ref_pts = [np.exp(1j * 2 * np.pi * n / 8 + 1j * np.pi/8) for n in range(8)]
            elif mod == "16-QAM": ref_pts = [x + 1j*y for x in [-3,-1,1,3] for y in [-3,-1,1,3]]
            elif mod == "64-QAM": ref_pts = [x + 1j*y for x in [-7,-5,-3,-1,1,3,5,7] for y in [-7,-5,-3,-1,1,3,5,7]]
            
            if ref_pts:
                pts = np.array(ref_pts) * self.ref_scale.get()
                self.ax.scatter(pts.real, pts.imag, marker='x', s=100, color='red', label='Ref')
            self.ax.set_aspect('equal', 'datalim'); self.ax.grid(True, alpha=0.2); self.canvas.draw()
        except: pass

class SpectrogramWindow(tk.Toplevel):
    def __init__(self, parent, data, fs):
        super().__init__(parent)
        self.title("Spectral Waterfall"); self.geometry("1100x900"); self.data, self.fs = data, fs
        ctrl = ttk.LabelFrame(self, text="Spectrogram Parameters"); ctrl.pack(fill="x", padx=10, pady=5)
        r1 = ttk.Frame(ctrl); r1.pack(fill="x", padx=5, pady=2)
        self.is_frozen = tk.BooleanVar(value=False); ttk.Checkbutton(r1, text="FREEZE", variable=self.is_frozen).pack(side="left")
        ttk.Label(r1, text="NFFT:").pack(side="left"); self.nfft = tk.StringVar(value="1024"); ttk.Entry(r1, textvariable=self.nfft, width=8).pack(side="left", padx=5)
        ttk.Label(r1, text="Noverlap:").pack(side="left", padx=5); self.noverlap = tk.StringVar(value="512"); ttk.Entry(r1, textvariable=self.noverlap, width=8).pack(side="left")
        ttk.Label(r1, text="Window:").pack(side="left", padx=5); self.win_type = tk.StringVar(value="hann")
        ttk.Combobox(r1, textvariable=self.win_type, values=["hann", "hamming", "blackman", "boxcar"], width=10).pack(side="left")
        r2 = ttk.Frame(ctrl); r2.pack(fill="x", padx=5, pady=2)
        ttk.Label(r2, text="Colormap:").pack(side="left"); self.cmap_var = tk.StringVar(value="magma")
        ttk.Combobox(r2, textvariable=self.cmap_var, values=["magma", "viridis", "inferno", "jet"], width=10).pack(side="left", padx=5)
        ttk.Label(r2, text="Min dB:").pack(side="left", padx=5); self.min_db = tk.StringVar(value="-120"); ttk.Entry(r2, textvariable=self.min_db, width=6).pack(side="left")
        ttk.Button(r2, text="Update Spectrogram", command=self.plot_spec).pack(side="right", padx=10)
        self.fig, self.ax = plt.subplots(); self.canvas = FigureCanvasTkAgg(self.fig, master=self)
        self.canvas.get_tk_widget().pack(fill="both", expand=True); self.plot_spec()

    def plot_spec(self):
        try:
            self.ax.clear(); n = int(self.nfft.get()); nov = int(self.noverlap.get())
            f, t, Sxx = signal.spectrogram(self.data, self.fs, window=self.win_type.get(), nperseg=n, noverlap=nov, return_onesided=False)
            f, Sxx = np.fft.fftshift(f), np.fft.fftshift(Sxx, axes=0)
            self.ax.pcolormesh(t, f, 10 * np.log10(Sxx + 1e-18), shading='gouraud', cmap=self.cmap_var.get(), vmin=float(self.min_db.get()))
            self.ax.set_ylabel("Freq [Hz]"); self.ax.set_xlabel("Time [s]"); self.canvas.draw()
        except: pass

if __name__ == "__main__":
    root = tk.Tk(); app = DataVisualizerApp(root); root.mainloop()