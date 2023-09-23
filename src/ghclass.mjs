import { parse } from 'csv-parse/sync';
import fs from 'node:fs/promises';
import { exec } from 'node:child_process';
import util from 'node:util';

const execp = util.promisify(exec);

/**
 * Returns a GitHub url using the ssh protocol
 * @param {object} student A populated student object
 * @param {string} organization A valid GitHub organization
 * @param {string} project A project identifier
 * @return {string} A string formatted to clone a github repo over SSH
 */
export function gitHubRepoUrlSSH(student, organization, project) {
  return `git@github.com:${
    organization}/${project}-${
    student.team || student.githubUserName
  } ${student.team || student.email}`;
}

/**
 * Loads a roster that was exported from classrooms.github.com
 * and returns an array of student objects
 * @param {string} path Path to file
 * @return {Array} An array containing the all the students
 */
export async function loadRosterCSV(path) {
  const fle = await fs.readFile(path, { encoding: 'utf-8' });
  const csv = parse(fle);
  const rval = [];
  // Groups can have spaces in them and need to be replaced with -
  // to match the format that classrooms is using
  csv.slice(1).forEach((student) => {
    const tmp = {
      email: student[0],
      githubUserName: student[1],
      githubID: student[2],
      name: student[3],
      team: student.length === 5 ? student[4].replaceAll(' ', '-') : null,
    };
    rval.push(tmp);
  });
  return rval;
}

/**
 * Clones all project repos for a given class. If roster is set to 'none' then
 * each repository will be clone using git's standard naming convention. If
 * roster points to a valid file then each repository will be cloned into a
 * directory matching the students roster id
 *
 * @param {string} org The github organization
 * @param {string} project The project name
 * @param {string} roster The path to the class roster
 */
export async function cloneAll(org, project, roster) {
  console.log(`Clone with options: ${org} ${project} ${roster}`);
  const students = await loadRosterCSV(roster);
  const clones = [];
  students.forEach((s) => {
    if (s.githubID !== '') {
      const r = gitHubRepoUrlSSH(s, org, project);
      clones.push(execp(`git clone ${r}`).catch(() => {
        console.log(`-->student: ${s.email} has not accepted the assignment`);
      }));
    } else {
      console.log(`student: ${s.email} has not accepted the assignment`);
    }
  });

  await Promise.allSettled(clones).then((results) => {
    results.forEach((result) => {
      if (result.value) {
        console.log(result.value.stderr.trim());
      }
    });
  });
}
