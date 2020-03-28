function rotateLeft(matrix) {
    const rows = matrix.length;
    const columns = matrix[0].length;
    const res = [...Array(rows)].map(_ => Array(columns));

    for (let row = 0; row < rows; row++) {
        for (let column = 0; column < columns; column++) {
            res[row][column] = matrix[column][columns - row - 1];
        }
    }
    return res;
};

class Tile {
    static uniqueID = 0;

    constructor(value = 0, row = -1, column = -1) {
        this.id = Tile.uniqueID++;

        this.value = value;
        this.row = row;
        this.column = column;

        this.oldRow = -1;
        this.oldColumn = -1;
        this.markForDeletion = false;
        this.mergedInto = null;
    }

    moveTo(row, column) {
        this.oldRow = this.row;
        this.oldColumn = this.column;
        this.row = row;
        this.column = column;
    }

    isNew() {
        return this.oldRow == -1 && !this.mergedInto;
    }

    hasMoved() {
        return (this.fromRow() != -1 && (this.fromRow() != this.toRow() || this.fromColumn() != this.toColumn())) || this.mergedInto;
    }

    fromRow() {
        return this.mergedInto ? this.row : this.oldRow;
    }

    fromColumn() {
        return this.mergedInto ? this.column : this.oldColumn;
    }

    toRow() {
        return this.mergedInto ? this.mergedInto.row : this.row;
    }

    toColumn() {
        return this.mergedInto ? this.mergedInto.column : this.column;
    }
}

class Board {
    constructor() {
        this.won = false;
        this.score = 0;
        this.size = 4;
        this.fourProbability = 0.1;

        this.tiles = [];
        this.cells = [];

        for (let i = 0; i < this.size; i++) {
            this.cells[i] = [];
            for (let j = 0; j < this.size; j++) {
                this.cells[i][j] = this.addTile();
            }
        }

        this.addRandomTile();
        this.setPositions();
    }

    addTile(value = 0) {
        const res = new Tile(value);
        this.tiles.push(res);
        return res;
    }

    moveLeft() {
        let hasChanged = false;
        for (let row = 0; row < this.size; ++row) {
            const currentRow = this.cells[row].filter(tile => tile.value != 0);
            const resultRow = [];

            for (let target = 0; target < this.size; ++target) {
                let targetTile = currentRow.length ? currentRow.shift() : this.addTile();

                if (currentRow.length > 0 && currentRow[0].value == targetTile.value) {
                    const tile1 = targetTile;
                    targetTile = this.addTile(targetTile.value);
                    tile1.mergedInto = targetTile;

                    const tile2 = currentRow.shift();
                    tile2.mergedInto = targetTile;
                    targetTile.value += tile2.value;

                    this.score += targetTile.value;
                }

                resultRow[target] = targetTile;
                this.won |= (targetTile.value == 2048);
                hasChanged |= (targetTile.value != this.cells[row][target].value);
            }
            this.cells[row] = resultRow;
        }
        return hasChanged;
    }

    setPositions() {
        this.cells.forEach((row, rowIndex) => {
            row.forEach((tile, columnIndex) => {
                tile.oldRow = tile.row;
                tile.oldColumn = tile.column;
                tile.row = rowIndex;
                tile.column = columnIndex;
                tile.markForDeletion = false;
            });
        });
    }

    addRandomTile() {
        const emptyCells = [];
        for (let r = 0; r < this.size; r++) {
            for (let c = 0; c < this.size; c++) {
                if (this.cells[r][c].value == 0) {
                    emptyCells.push({ r, c });
                }
            }
        }

        const index = Math.floor(Math.random() * emptyCells.length);
        const { r, c } = emptyCells[index];
        const newValue = Math.random() < this.fourProbability ? 4 : 2;

        this.cells[r][c] = this.addTile(newValue);
    }

    move(direction) {
        // 0 -> left, 1 -> up, 2 -> right, 3 -> down
        this.clearOldTiles();

        for (let i = 0; i < direction; i++) {
            this.cells = rotateLeft(this.cells);
        }

        const hasChanged = this.moveLeft();

        for (let i = direction; i < 4; i++) {
            this.cells = rotateLeft(this.cells);
        }

        if (hasChanged) {
            this.addRandomTile();
        }

        this.setPositions();
    }

    clearOldTiles() {
        this.tiles = this.tiles.filter(tile => !tile.markForDeletion);
        this.tiles.forEach(tile => { tile.markForDeletion = true; });
    }

    hasWon() {
        return this.won;
    }

    hasLost() {
        let canMove = false;
        const deltaX = [-1, 0, 1, 0];
        const deltaY = [0, -1, 0, 1];

        for (let row = 0; row < this.size; row++) {
            for (let column = 0; column < this.size; column++) {
                canMove |= (this.cells[row][column].value == 0);

                for (let dir = 0; dir < 4; dir++) {
                    const newRow = row + deltaX[dir];
                    const newColumn = column + deltaY[dir];

                    if (newRow < 0 || newRow >= this.size || newColumn < 0 || newColumn >= this.size) {
                        continue;
                    }

                    canMove |= (this.cells[row][column].value == this.cells[newRow][newColumn].value);
                }
            }
        }
        return !canMove;
    }
}

export { Board }
