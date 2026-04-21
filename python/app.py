from flask import Flask, render_template, jsonify
import subprocess
import json
import os

app = Flask(__name__, template_folder='../frontend')

def get_portfolio_data():
    # This Calls my C++ executable from project root
    result = subprocess.run(
        ['./cpp/a.out'],
        capture_output=True,
        text=True
    )

    #print("C++ output:")
    #print(result.stdout)

    if result.returncode != 0:
        print("C++ error", result.stderr)
        return None
    
    data = json.loads(result.stdout)
    return data



@app.route('/')
def index():
    data = get_portfolio_data()
    return render_template('index.html', data=data)


@app.route('/api/portfolio')
def api_portfolio():
    data = get_portfolio_data()
    return jsonify(data)

if __name__ == '__main__':
    app.run(debug=True)