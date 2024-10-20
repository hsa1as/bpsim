import subprocess
import re
import os

def run_simulation(predictor_type, m, n=None, trace_file=''):
    if predictor_type == 'bimodal':
        cmd = f"./bpsim bimodal {m} {trace_file}"
    else:
        cmd = f"./bpsim gshare {m} {n} {trace_file}"
    
    result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    output = result.stdout

    # Extract misprediction rate
    match = re.search(r'misprediction rate:\s+([\d.]+)%', output)
    if match:
        return float(match.group(1))
    else:
        return None

def generate_bimodal_data(trace_files):
    data = {trace: {} for trace in trace_files}
    for trace in trace_files:
        for m in range(7, 13):
            rate = run_simulation('bimodal', m, trace_file=trace)
            if rate is not None:
                data[trace][m] = rate
    return data

def generate_gshare_data(trace_files):
    data = {trace: {} for trace in trace_files}
    for trace in trace_files:
        for m in range(7, 13):
            for n in range(2, m + 1, 2):
                rate = run_simulation('gshare', m, n, trace_file=trace)
                if rate is not None:
                    if n not in data[trace]:
                        data[trace][n] = {}
                    data[trace][n][m] = rate
    return data

def generate_latex_plot(data, plot_type, trace_file):
    latex_code = f"""
\\begin{{figure}}[htbp]
\\centering
\\begin{{tikzpicture}}
\\begin{{axis}}[
    title={{{plot_type.capitalize()} Predictor Misprediction Rate for {trace_file}}},
    xlabel={{m}},
    ylabel={{Misprediction Rate (\%)}},
    legend pos=north east,
    ymajorgrids=true,
    grid style=dashed,
]
"""

    if plot_type == 'bimodal':
        latex_code += f"""
\\addplot[color=blue,mark=square] coordinates {{
    {' '.join([f'({m},{rate})' for m, rate in data[trace_file].items()])}
}};
\\legend{{Bimodal}}
"""
    else:  # gshare
        colors = ['red', 'blue', 'green', 'orange', 'purple', 'brown']
        for i, (n, m_data) in enumerate(data[trace_file].items()):
            latex_code += f"""
\\addplot[color={colors[i % len(colors)]},mark=square] coordinates {{
    {' '.join([f'({m},{rate})' for m, rate in m_data.items()])}
}};
"""
        latex_code += f"""
\\legend{{{', '.join([f'n={n}' for n in data[trace_file].keys()])}}}
"""

    latex_code += """
\\end{axis}
\\end{tikzpicture}
\\end{figure}
"""

    return latex_code

def main():
    trace_files = ['gcc_trace.txt', 'jpeg_trace.txt']
    
    # Generate bimodal data and plots
    bimodal_data = generate_bimodal_data(trace_files)
    bimodal_latex = ''
    for trace in trace_files:
        bimodal_latex += generate_latex_plot(bimodal_data, 'bimodal', trace)
    
    # Generate gshare data and plots
    gshare_data = generate_gshare_data(trace_files)
    gshare_latex = ''
    for trace in trace_files:
        gshare_latex += generate_latex_plot(gshare_data, 'gshare', trace)
    
    # Combine all LaTeX code
    full_latex = f"""
\\documentclass{{article}}
\\usepackage{{pgfplots}}
\\pgfplotsset{{compat=1.18}}

\\begin{{document}}

\\section{{Bimodal Predictor Results}}
{bimodal_latex}

\\section{{Gshare Predictor Results}}
{gshare_latex}

\\end{{document}}
"""

    # Write LaTeX code to file
    with open('branch_predictor_plots.tex', 'w') as f:
        f.write(full_latex)

    print("LaTeX file 'branch_predictor_plots.tex' has been generated.")

if __name__ == "__main__":
    main()
