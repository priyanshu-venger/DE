from flask import Flask, request, jsonify, flash, redirect, url_for , render_template, session , make_response
from flask_pymongo import PyMongo, ObjectId
from werkzeug.utils import secure_filename
from gridfs import GridFS
import random
import email, smtplib, ssl, os
from bson import ObjectId
from email import encoders
from email.mime.base import MIMEBase
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from bson.errors import InvalidId
import json
from flask_cors import CORS
app = Flask(__name__)
CORS(app)
connection_string="mongodb+srv://anshu22143:0mkPp6aGyVWGBC50@cluster0.kcz1v.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0"
app.config["MONGO_URI"] = connection_string
app.config['SECRET_KEY'] = os.environ.get('SECRET_KEY', 'optional_default')
mongo = PyMongo(app)
db=mongo.cx['DE']
fs = GridFS(db)
@app.route('/')
async def home():
    if not session.get('logged_in'):
        return redirect(url_for('login'))
    if session.get('logged_in'):
        flash("You're already logged in","message")
    session['logged_in']=True
    if session.get('role')=='NGO':
        return render_template('NGO.html',id=session.get('id'))

    if session.get('role')=="Corp":
        return render_template('Corp.html',id=session.get('id'))
    
    return render_template('User.html',id=session.get('id'))

@app.route('/login',methods=['POST','GET'])
async def login():
    if request.method == 'POST':
        if not session.get('logged_in'):
            id=request.form.get('id')
            pswd=request.form.get('password')
            user=db.users.find_one({'id':id,'password':pswd})
            if not user:
                flash('Invalid ID or password', 'error')
                return redirect(url_for('login'))
            session['id']=id
            session['role']=user.get('role')
        return redirect(url_for('home'))
    return render_template('login.html')

@app.route('/logout')
async def logout():
    session.pop('id')
    session.pop('logged_in')
    session.pop('role')
    return redirect(url_for('login'))

@app.route('/signup',methods=['POST','GET'])
async def signup():
    if request.method=="POST":
        id=request.form.get('id')
        pswd=request.form.get('password')
        name=request.form.get('name')
        role=request.form.get('role')
        if db.users.find_one({'id':id,'password':pswd}):
            flash('ID already exists', 'error')
            return redirect(url_for('signup'))

        db.users.insert_one({'id':id,'password':pswd,'name':name,'role':role})
        return redirect(url_for('login'))
    return render_template('signup.html')

@app.route('/upload',methods=['GET','POST'])
def upload_report():
    if request.method=="POST":
        if session.get('role')!='NGO':
            flash('You are not allowed to access this page.', 'error')
            return redirect(url_for('home'))
        file=request.files['file']
        file_id = fs.put(file, filename=file.filename, content_type=file.content_type)
        db.reports.insert_one({'id':session.get('id'),'report':file_id})
        flash('Report uploaded successfully', 'success')
        return redirect(url_for('home'))
    return render_template('upload_report.html')
    
@app.route('/file/<NGO_id>',methods=['GET','POST'])
def file(ngo_id):
    if not session.get('role'):
        flash('You need to be logged in for accessing this file','error')
    file_id=db.reports.find_one({'id':ngo_id}).get('report')
    file = fs.get(ObjectId(file_id))
    response = make_response(file.read())
    response.mimetype = file.content_type
    return response

@app.route('/profile')
def profile():
    if not session.get('logged_in'):
        flash('You need to be logged in for accessing this page','error')
        return redirect(url_for('login'))
    
    id=session.get('id')
    user=db.users.find_one({'id':id})
    return render_template('profile.html',user=user)



if __name__ == "__main__":
    app.run(debug=True)
    
