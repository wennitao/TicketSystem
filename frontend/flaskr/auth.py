import functools
import os

from flask import (
    Blueprint, flash, g, redirect, render_template, request, session, url_for
)
from werkzeug.security import check_password_hash, generate_password_hash

bp = Blueprint('auth', __name__, url_prefix='/auth')

@bp.route('/register', methods=('GET', 'POST'))
def register():
    if request.method == 'POST':
        cur_username = None
        if g.user is None:
            cur_username = "Empty_Name!!!"
        else:
            cur_username = g.user.username
        username = request.form['username']
        password = request.form['password']
        name = request.form['name']
        mailAddr = request.form['mailAddress']
        privilege = request.form['privilege']
        error = None

        fileio = open ("command.in", "w")
        fileio.write ("add_user -c " + cur_username + ' -u ' + username + ' -p ' + password + ' -n ' + name + ' -m ' + mailAddr + ' -g ' + privilege)
        fileio.close()
        os.system ("./code < command.in > command.out")
        fileio = open ("command.out", "r")
        result = int (fileio.read ())
        fileio.close()
        
        if result == 0:
            return redirect(url_for('auth.login'))
        else:
            error = "register failed"

        flash(error)

    return render_template('auth/register.html')

def dataBase_queryUser(username):
    fileio = open ("command.in", "w")
    fileio.write ("query_profile -c " + username + " -u " + username)
    fileio.close()
    os.system ("./code < command.in > command.out")
    fileio = open ("command.out", "r")
    res_str = fileio.read ()
    print (res_str)
    user = dict ()
    user['username'], user['name'], user['mailAddr'], user['privilege'] = res_str.split (' ')
    return user

def dataBase_login(username, password):
    print ("login " + username + " " + password)
    fileio = open ("command.in", "w")
    fileio.write ("login -u " + username + " -p " + password)
    fileio.close()
    os.system ("./code < command.in > command.out")
    fileio = open ("command.out", "r")
    result = int (fileio.read ())
    print (result)
    fileio.close()
    if result == -1:
        return None
    else:
        return dataBase_queryUser (username)


@bp.route('/login', methods=('GET', 'POST'))
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        error = None
        user = dataBase_login (username, password)

        if user is None:
            error = 'loggin failed'

        if error is None:
            session.clear()
            session['user_id'] = user['username']
            return redirect(url_for('main.index'))

        flash(error)

    return render_template('auth/login.html')

@bp.before_app_request
def load_logged_in_user():
    user_id = session.get('user_id')

    if user_id is None:
        g.user = None
    else:
        g.user = dataBase_queryUser (user_id)
    
@bp.route('/logout')
def logout():
    session.clear()
    return redirect(url_for('index'))

def login_required(view):
    @functools.wraps(view)
    def wrapped_view(**kwargs):
        if g.user is None:
            return redirect(url_for('auth.login'))

        return view(**kwargs)

    return wrapped_view