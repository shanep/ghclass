import { parse } from 'csv-parse/sync'
import fs from 'node:fs/promises'

/**
 * Returns a GitHub url using the ssh protocol
 * @param {object} student A populated student object
 * @param {string} organization A valid GitHub organization
 * @param {string} project A project identifier
 * @returns A string formatted to clone a github repo over SSH
 */
export function gitHubRepoUrlSSH (student, organization, project) {
    return "git@github.com:" + organization + "/" +
        project + "-" + (student.team || student.githubUserName);
}

/**
 * Loads a roster that was exported from classrooms.github.com
 * and returns an array of student objects
 * @param {string} path Path to file
 * @returns An array containing the all the students
 */
export async function loadRosterCSV(path) {
    const fle = await fs.readFile(path, { encoding: 'utf-8' });
    const csv = parse(fle);
    let rval = [];
    //Groups can have spaces in them and need to be replaced with -
    //to match the format that classrooms is using
    for (let student of csv.slice(1)) {
        let tmp = {
            email: student[0],
            githubUserName: student[1],
            githubID: student[2],
            name: student[3],
            team: student.length === 5 ? student[4].replaceAll(' ', '-') : null
        }
        rval.push(tmp);
    }
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
    console.log("Clone with options: " + org + " " + project + " " + roster);
    const students = await loadRosterCSV(roster);
    for (s of students){
        console.log("Cloning: " + gitHubRepoUrlSSH(s,org,project));
    }
}