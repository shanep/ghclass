import { parse } from 'csv-parse/sync';
import * as fs from 'node:fs/promises';
import { exec } from 'node:child_process';
import * as util from 'node:util';

const execp = util.promisify(exec);

/**
 * A student object that contains the information needed to clone a repo
 */
export type Student = {
  email: string | undefined,
  githubUserName: string | undefined,
  githubID: string | undefined,
  name: string | undefined,
  team: string | undefined
};

/**
 * Returns a GitHub url using the ssh protocol
 * @param {object} student A populated student object
 * @param {string} organization A valid GitHub organization
 * @param {string} project A project identifier
 * @return {string} A string formatted to clone a github repo over SSH
 */
export function gitHubRepoUrlSSH(student: Student, organization: string, project: string): string {
  return `git@github.com:${organization}/${project}-${student.team || student.githubUserName
    } ${student.team || student.email}`;
}

/**
 * Loads a roster that was exported from classrooms.github.com
 * and returns an array of student objects
 * @param {string} path Path to file
 * @return {Promise<Student[]>} An array containing the all the students
 */
export async function loadRosterCSV(path: string): Promise<Student[]> {
  const fle = await fs.readFile(path, { encoding: 'utf-8' });
  const csv: Array<Array<string>> = parse(fle);
  const rval: Student[] = [];
  // Groups can have spaces in them and need to be replaced with -
  // to match the format that classrooms is using
  csv.slice(1).forEach((student) => {
    const tmp: Student = {
      email: student[0],
      githubUserName: student[1],
      githubID: student[2],
      name: student[3],
      team: student[4]?.replaceAll(' ', '-')
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
 * 
 */
export async function cloneAll(org: string, project: string, roster: string) : Promise<number> {
  console.log(`Clone with options: ${org} ${project} ${roster}`);
  const students = await loadRosterCSV(roster);
  const clones: Promise<void | { stdout: string; stderr: string; }>[] = [];
  students.forEach((s) => {
    if (s.githubID !== '') {
      const r = gitHubRepoUrlSSH(s, org, project);
      clones.push(execp(`gh repo clone ${r}`).catch((err) => {
        console.log(err);
      }));
    } else {
      console.log(`student: ${s.email || s.githubUserName} has not accepted the assignment`);
    }
  });

  const results = await Promise.allSettled(clones)
  let count = 0;

  results.forEach((result) => {
    switch (result.status) {
      case 'rejected':
        if(result.reason){
          console.log(result.reason);
        }
        break;
      case 'fulfilled':
        if(result.value){
          count++;
          console.log(result.value.stderr.trim())
        }
        break;
      default:
        console.log("Got an unknown case on a Promise!");
    }
  });
  return count;
}