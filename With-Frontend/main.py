from flask import Flask, render_template, request, redirect, url_for, flash
import subprocess
import os
import tempfile
from datetime import datetime

app = Flask(__name__)
app.secret_key = 'your_secret_key_here'

C_PROGRAM_PATH = './trans3.c'

def execute_c_program(choice, account=None, amount=None, name=None, backup_file=None):
    try:
        cmd = [C_PROGRAM_PATH]
        
        input_data = f"{choice}\n"
        
        if choice in [2, 3, 4, 5, 7]:
            input_data += f"{account}\n"
            if choice == 2:
                input_data += f"{amount}\n"
            elif choice == 3:
                input_data += f"{name.split()[1]}\n{name.split()[0]}\n{amount}\n"
        
        elif choice == 6:
            input_data += f"{name}\n"
        
        elif choice == 10:
            input_data += f"{backup_file}\ny\n"
        
        with tempfile.NamedTemporaryFile(mode='w+', delete=False) as input_file:
            input_file.write(input_data)
            input_file_path = input_file.name
        
        result = subprocess.run(
            cmd,
            stdin=open(input_file_path, 'r'),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        os.unlink(input_file_path)
        
        if result.returncode != 0:
            return False, f"Error executing C program: {result.stderr}"
        
        return True, result.stdout
        
    except Exception as e:
        return False, str(e)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/create_text_file')
def create_text_file():
    success, output = execute_c_program(1)
    if success:
        flash('Text file created successfully!', 'success')
    else:
        flash(f'Error creating text file: {output}', 'danger')
    return redirect(url_for('index'))

@app.route('/update_record', methods=['GET', 'POST'])
def update_record():
    if request.method == 'POST':
        account = request.form['account']
        amount = request.form['amount']
        success, output = execute_c_program(2, account=account, amount=amount)
        if success:
            flash(f'Account updated successfully! Output: {output}', 'success')
        else:
            flash(f'Error updating account: {output}', 'danger')
        return redirect(url_for('index'))
    return render_template('update_record.html')

@app.route('/add_account', methods=['GET', 'POST'])
def add_account():
    if request.method == 'POST':
        account = request.form['account']
        first_name = request.form['first_name']
        last_name = request.form['last_name']
        balance = request.form['balance']
        name = f"{first_name} {last_name}"
        success, output = execute_c_program(3, account=account, amount=balance, name=name)
        if success:
            flash(f'Account added successfully! Output: {output}', 'success')
        else:
            flash(f'Error adding account: {output}', 'danger')
        return redirect(url_for('index'))
    return render_template('add_account.html')

@app.route('/delete_account', methods=['GET', 'POST'])
def delete_account():
    if request.method == 'POST':
        account = request.form['account']
        success, output = execute_c_program(4, account=account)
        if success:
            flash(f'Account deleted successfully! Output: {output}', 'success')
        else:
            flash(f'Error deleting account: {output}', 'danger')
        return redirect(url_for('index'))
    return render_template('delete_account.html')

@app.route('/view_account', methods=['GET', 'POST'])
def view_account():
    if request.method == 'POST':
        account = request.form['account']
        success, output = execute_c_program(5, account=account)
        if success:
            return render_template('view_account_result.html', output=output)
        else:
            flash(f'Error viewing account: {output}', 'danger')
            return redirect(url_for('index'))
    return render_template('view_account.html')

@app.route('/search_by_name', methods=['GET', 'POST'])
def search_by_name():
    if request.method == 'POST':
        name = request.form['name']
        success, output = execute_c_program(6, name=name)
        if success:
            return render_template('search_results.html', output=output)
        else:
            flash(f'Error searching accounts: {output}', 'danger')
            return redirect(url_for('index'))
    return render_template('search_by_name.html')

@app.route('/view_transaction_history', methods=['GET', 'POST'])
def view_transaction_history():
    if request.method == 'POST':
        account = request.form['account']
        success, output = execute_c_program(7, account=account)
        if success:
            return render_template('transaction_history.html', output=output)
        else:
            flash(f'Error viewing transaction history: {output}', 'danger')
            return redirect(url_for('index'))
    return render_template('view_transaction_history.html')

@app.route('/generate_summary')
def generate_summary():
    success, output = execute_c_program(8)
    if success:
        return render_template('account_summary.html', output=output)
    else:
        flash(f'Error generating summary: {output}', 'danger')
        return redirect(url_for('index'))

@app.route('/backup_accounts')
def backup_accounts():
    success, output = execute_c_program(9)
    if success:
        flash(f'Backup created successfully! Output: {output}', 'success')
    else:
        flash(f'Error creating backup: {output}', 'danger')
    return redirect(url_for('index'))

@app.route('/restore_backup', methods=['GET', 'POST'])
def restore_backup():
    if request.method == 'POST':
        backup_file = request.form['backup_file']
        success, output = execute_c_program(10, backup_file=backup_file)
        if success:
            flash(f'Backup restored successfully! Output: {output}', 'success')
        else:
            flash(f'Error restoring backup: {output}', 'danger')
        return redirect(url_for('index'))
    
    backup_files = [f for f in os.listdir('.') if f.startswith('clients_backup_') and f.endswith('.dat')]
    return render_template('restore_backup.html', backup_files=backup_files)

if __name__ == '__main__':
    app.run(debug=True)
