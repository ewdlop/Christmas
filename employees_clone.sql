CREATE TABLE employees (
    id INT AUTOINCREMENT,
    name STRING,
    department STRING,
    hire_date DATE
);

INSERT INTO employees (name, department, hire_date) VALUES
('Alice', 'Engineering', '2023-01-15'),
('Bob', 'Marketing', '2023-02-20'),
('Charlie', 'HR', '2023-03-10');

SELECT * FROM employees WHERE department = 'Engineering';

CREATE TABLE employees_clone CLONE employees;

CREATE TABLE json_data (data VARIANT);

INSERT INTO json_data (data) VALUES
(parse_json('{"name": "Dave", "age": 30, "city": "New York"}'));

SELECT data:name::STRING AS name, data:age::INT AS age FROM json_data;

CREATE EXTERNAL TABLE external_data
  WITH LOCATION = 's3://my-bucket/path/'
  FILE_FORMAT = (TYPE = 'PARQUET');

SELECT * FROM external_data;
