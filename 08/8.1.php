<?php

$fileName = __DIR__ . '/8.txt';

class Entity
{
    /** @var int */
    public $name;

    /** @var Entity[] */
    public $childs = [];

    /** @var int[] */
    public $metadata = [];

    /** @var Entity */
    public $parent;
}

class Parser
{
    /** @var array */
    protected $licenseParts;

    /** @var int */
    protected $crtIndex;

    /** @var int */
    protected $licenseLength;

    /** @var Entity */
    protected $root;

    /** @var Entity[] */
    protected $entities = [];

    /** @var Entity */
    protected $crtEntity;

    /** @var int */
    protected $metadataSum;

    /**
     * @param string $license
     */
    public function __construct(string $license)
    {
        $this->licenseParts = explode(' ', trim($license));
        $this->licenseLength = count($this->licenseParts);
    }

    public function parse(): void
    {
        $this->crtIndex = 0;
        $this->metadataSum = 0;
        $this->parseEntity();

        var_dump($this->metadataSum);
    }

    protected function parseEntity(): void
    {
        $entity = new Entity();

        if ($this->crtEntity) {
            $entity->name = count($this->entities) + 1;
            $entity->parent = $this->crtEntity;
            $this->crtEntity->childs[] = $entity;
        } else {
            $entity->name = 1;
            $this->root = $entity;
        }

        $this->crtEntity = $entity;
        $this->entities[$entity->name] = $entity;

        $childs = (int) $this->licenseParts[$this->crtIndex++];
        $metadata = (int) $this->licenseParts[$this->crtIndex++];

        for ($i = 0; $i < $childs; $i++) {
            $this->parseEntity();
        }

        for ($i = 0; $i < $metadata; $i++) {
            $m = (int) $this->licenseParts[$this->crtIndex++];
            $this->metadataSum += $m;
            $entity->metadata[] = $m;
        }

        $this->crtEntity = $entity->parent;
    }
}

(new Parser(file_get_contents($fileName)))->parse();
